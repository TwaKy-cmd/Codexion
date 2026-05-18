/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twaky <twaky@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 21:58:10 by twaky             #+#    #+#             */
/*   Updated: 2026/05/18 22:19:48 by twaky            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

t_heap	*heap_create(int capacity)
{
	t_heap	*heap;

	heap = malloc(sizeof(t_heap));
	if (!heap)
		return (NULL);
	heap->nodes = malloc(sizeof(t_heap_node) * capacity);
	if (!heap->nodes)
	{
		free(heap);
		return (NULL);
	}
	heap->size = 0;
	heap->capacity = capacity;
	return (heap);
}

int	heap_push(t_heap *heap, int coder_id, long long key)
{
	int			i;
	int			parent;
	t_heap_node	tmp;

	if (!heap || heap->size >= heap->capacity)
		return (0);
	heap->nodes[heap->size].coder_id = coder_id;
	heap->nodes[heap->size].key = key;
	i = heap->size;
	heap->size++;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (heap->nodes[i].key >= heap->nodes[parent].key)
			break ;
		tmp = heap->nodes[i];
		heap->nodes[i] = heap->nodes[parent];
		heap->nodes[parent] = tmp;
		i = parent;
	}
	return (1);
}

int	get_smallest(t_heap *heap, int i)
{
	int	left;
	int	right;
	int	smallest;

	left = 2 * i + 1;
	right = 2 * i + 2;
	if (left >= heap->size)
		return (i);
	smallest = i;
	if (heap->nodes[left].key < heap->nodes[smallest].key)
		smallest = left;
	if (right < heap->size
		&& heap->nodes[right].key < heap->nodes[smallest].key)
		smallest = right;
	return (smallest);
}

int	heap_pop(t_heap *heap, int *out_coder_id)
{
	int	i;
	int	smallest;

	if (!heap || heap->size == 0)
		return (0);
	*out_coder_id = heap->nodes[0].coder_id;
	heap->nodes[0] = heap->nodes[heap->size-- - 1];
	i = 0;
	while (1)
	{
		smallest = get_smallest(heap, i);
		if (smallest == i)
			break ;
		i = swap(heap, i, smallest);
	}
	return (1);
}
