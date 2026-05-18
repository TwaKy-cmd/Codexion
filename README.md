*This project has been created as part of the 42 curriculum by twaky.*

# Codexion

## Description

Codexion is a multithreaded simulation inspired by the classic **Dining Philosophers** problem, reframed in a software development context. A group of coders sit around a circular table, each needing two **dongles** (USB license keys) — one on their left and one on their right — to compile their code. Since each dongle is shared between two adjacent coders, they must coordinate access to avoid deadlock, starvation, and burnout.

Each coder cycles through four states:

1. **Taking dongles** — acquiring the two adjacent shared resources
2. **Compiling** — holding both dongles for `time_to_compile` ms
3. **Debugging** — releasing dongles, sleeping for `time_to_debug` ms
4. **Refactoring** — resting for `time_to_refactor` ms before the next cycle

The simulation ends in one of two outcomes:
- **Success**: every coder has completed `compiles_required` compilations
- **Burnout**: a coder has not started a new compilation within `time_to_burnout` ms

The project ships with two scheduling policies (**FIFO** and **EDF**) for dongle acquisition, implemented via a per-dongle min-heap priority queue.

## Instructions

### Requirements

- A C compiler (`cc`)
- POSIX threads library (`pthread`)
- GNU Make

### Compilation

```bash
make
```

This produces the `codexion` binary. Use `make clean` to remove object files, `make fclean` to also remove the binary, and `make re` to rebuild from scratch.

### Execution

```
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor compiles_required dongle_cooldown scheduler
```

| Argument             | Description                                                    |
|----------------------|----------------------------------------------------------------|
| `number_of_coders`   | Number of coder threads (≥ 1)                                  |
| `time_to_burnout`    | Milliseconds before a coder burns out without compiling (≥ 1) |
| `time_to_compile`    | Milliseconds spent compiling (≥ 1)                             |
| `time_to_debug`      | Milliseconds spent debugging (≥ 1)                             |
| `time_to_refactor`   | Milliseconds spent refactoring (≥ 1)                           |
| `compiles_required`  | Number of compilations each coder must complete (≥ 0)          |
| `dongle_cooldown`    | Cooldown in ms before a released dongle can be taken again (≥ 0) |
| `scheduler`          | Dongle acquisition policy: `fifo` or `edf`                     |

### Examples

```bash
# Test 1 : run normal FIFO
./codexion 5 2000 200 200 200 5 100 fifo

# Test 2 : run normal EDF
./codexion 5 2000 200 200 200 5 100 edf

# Test 3 : burnout
./codexion 5 410 200 200 200 100 50 fifo

# Test 4 : 1 coder (burnout and stop)
./codexion 1 800 200 200 200 5 50 fifo

# Test 5 : 50 coders
./codexion 50 5000 100 100 100 3 50 fifo

# Test 6 : args not valid
./codexion
./codexion 5 800 200 200 200 10 50 truc
./codexion 5 -800 200 200 200 10 50 fifo
./codexion 0 800 200 200 200 10 50 fifo
./codexion abc 800 200 200 200 10 50 fifo
```

### Output format

Every state change is printed to stdout as:

```
<timestamp_ms> <coder_id> <action>
```

Examples:
```
0 3 has taken a dongle
0 3 has taken a dongle
0 3 is compiling
200 3 is debugging
300 3 is refactoring
350 3 burned out
```

## Blocking Cases Handled

### Deadlock prevention

**Deadlock** is a phenomenon that occurs when, for example, we have 4 coders who each take the right or left dongle and then wait for the left one, except that each coder holds it. So, it’s not possible and it produces infinite waiting that ends up creating the **Deadlock**. For that, the solution is to use the asymmetry which consists of:

- **Even-ID coders** take their **left** dongle first, then their right.
- **Odd-ID coders** take their **right** dongle first, then their left.

### Starvation prevention — priority queue scheduling

Without ordering, a coder could repeatedly lose the race to acquire a dongle to faster or luckier neighbours, never progressing. Two scheduling strategies are available, both implemented through a **per-dongle min-heap priority queue**:

- **FIFO** (`fifo`): the key pushed onto the heap is the current timestamp (`get_time_ms()`). The coder that has been waiting the longest (smallest key) is served first.
- **EDF — Earliest Deadline First** (`edf`): the key is `last_compile_start + time_to_burnout`, i.e., the absolute deadline by which the coder must compile. The coder closest to burning out is served first, ensuring no coder is starved to death.

Only the coder at the front of the heap (lowest key) is eligible to take the dongle. All others block on the condition variable and retry when woken.

### Cooldown handling

`dongle_cooldown` enforces a minimum delay between a dongle being released and being taken again. This prevents the same coder from immediately reclaiming its own dongle after releasing it, giving adjacent coders a guaranteed window to acquire it.

When a coder is first in the priority queue but the cooldown has not yet elapsed, it uses `pthread_cond_timedwait` to sleep **exactly** until the cooldown expires, then retries. This avoids busy-waiting while still reacting promptly.

### Burnout detection

A dedicated **monitor thread** (`monitor_routine`) polls every millisecond. For each coder it locks `state_mutex`, reads `last_compile_start`, and checks whether `now - last_compile_start > time_to_burnout`. The mutex prevents the monitor from reading a value that the coder is in the middle of updating (e.g., right after finishing a compile and before writing the new timestamp).

When burnout is detected:
1. The burnout state is logged immediately.
2. `set_sim_ended` raises the `sim_ended` flag under `end_mutex`.
3. `wake_all_dongles` calls `pthread_cond_broadcast` on every dongle's condition variable, unblocking any coder threads sleeping in `pthread_cond_wait` so they can check the flag and exit cleanly.

The same sequence runs when `check_all_done` confirms every coder has reached `compiles_required` compilations.

### Log serialization

`log_mutex` is held for the entire duration of each `printf` call inside `log_state`. This guarantees that lines from concurrent threads are never interleaved in the output, and that the timestamp printed always corresponds to the event being logged.

## Thread Synchronization Mechanisms

### `pthread_mutex_t dongle.mutex`

One mutex per dongle protects its entire mutable state: `is_taken`, `release_time`, and the priority queue. A coder must hold this mutex to read or write any of these fields, and keeps it locked for the full duration of the acquisition attempt — from pushing its key onto the heap to actually marking `is_taken = 1`. It releases the mutex only after breaking out of the acquisition loop (or when the simulation ends).

### `pthread_cond_t dongle.cond`

Paired with `dongle.mutex`, this condition variable allows a coder to block efficiently inside `take_one_dongle` while it cannot acquire the dongle:

- If the dongle is taken or another coder has priority, the thread calls `pthread_cond_wait` and releases the mutex atomically until it is woken.
- If the coder has priority but the cooldown has not expired, it calls `pthread_cond_timedwait` with a timeout computed to expire exactly when the cooldown ends.

When a coder releases its dongles (`release_dongles`), it calls `pthread_cond_broadcast` on both dongles' condition variables, waking all waiting coders so they can re-evaluate their eligibility.

**Race condition prevented**: without the mutex + condition variable pairing, a coder could check `is_taken == 0`, be preempted before calling `cond_wait`, miss the broadcast from the releasing thread, and sleep indefinitely. The atomicity of `pthread_cond_wait` (release mutex + sleep) eliminates this lost-wakeup race.

### `pthread_mutex_t sim.end_mutex`

Protects the shared `sim_ended` flag, read by every coder thread (`check_sim_ended`) and written by the monitor thread (`set_sim_ended`). Without this mutex, a coder could read a partially written integer on architectures that do not guarantee atomic word access, or observe a stale cached value due to compiler/CPU reordering.

### `pthread_mutex_t sim.log_mutex`

Serializes all output. Without it, two threads calling `printf` simultaneously could produce interleaved lines such as:
```
1200 1 is com150 3 is debugpiling
ging
```
Every call to `log_state` acquires this mutex before printing and releases it immediately after, ensuring atomic log lines.

### `pthread_mutex_t coder.state_mutex`

Protects `compile_count` and `last_compile_start` on each coder struct. These fields are written by the coder thread (after a compile completes) and read by the monitor thread (to detect burnout and check completion). The mutex prevents the monitor from reading a torn value or a value that is inconsistent with the coder's actual progress.

### Min-heap priority queue (custom)

Each dongle embeds a `t_heap *queue`, a binary min-heap that imposes a total ordering on waiting coders. It is always accessed while the dongle's mutex is held, so no additional synchronization is needed for the heap itself. The heap provides:
- `heap_push` — O(log n) insertion when a coder starts waiting
- `heap_peek` — O(1) query of the highest-priority coder
- `heap_pop` — O(log n) removal when the dongle is granted

This structure is the core mechanism that makes both FIFO and EDF scheduling lock-free from the coder's perspective: instead of iterating over all waiters, the dongle simply compares the waiting coder's ID with the heap's front element.

## Resources

### Classic references

- **E.W. Dijkstra** — *Solution of a Problem in Concurrent Programming Control* (1965) — the original mutual exclusion paper, foundational for the dining philosophers formulation.
- **E.W. Dijkstra** — *Hierarchical Ordering of Sequential Processes* (1971) — first formal statement of the Dining Philosophers problem.
- **The Open Group** — [*POSIX Threads Programming*](https://hpc-tutorials.llnl.gov/posix/) — comprehensive reference for `pthread` primitives used throughout this project.
- **cppreference / man pages** — `pthread_mutex_t`, `pthread_cond_t`, `pthread_cond_timedwait`, `gettimeofday`.
- **Wikipedia** — [*Deadlock*](https://en.wikipedia.org/wiki/Deadlock) — overview of Coffman's four necessary conditions.
- **Wikipedia** — [*Earliest Deadline First scheduling*](https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling) — background on the EDF policy implemented here.
- **C. L. Liu & J. W. Layland** — *Scheduling Algorithms for Multiprogramming in a Hard-Real-Time Environment* (1973) — theoretical basis for EDF.

### AI usage

Claude (claude-sonnet-4-6) was used as an assistant during the development of this project for the following tasks:

- **Debugging race conditions**: describing observed behaviours and asking for help reasoning about which mutex or condition variable was missing or misused.
- **Reviewing the dongle acquisition logic**: verifying that the combination of `pthread_cond_wait` / `pthread_cond_timedwait` and the priority queue correctly handled the cooldown edge case without busy-waiting.
- **Writing this README**: generating the first draft of all sections based on a reading of the source files, then refining the wording.

AI was not used to write the core simulation logic, the synchronization primitives setup, or the scheduling algorithms. All architectural decisions (asymmetric lock ordering, per-dongle heap, EDF key computation) were designed and implemented by the author.
