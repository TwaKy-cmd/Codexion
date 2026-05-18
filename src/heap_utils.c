/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twaky <twaky@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 16:12:51 by twaky             #+#    #+#             */
/*   Updated: 2026/05/18 22:41:17 by twaky            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void	heap_destroy(t_heap *heap)
{
	if (!heap)
		return ;
	free(heap->nodes);
	free(heap);
}

int	swap(t_heap *heap, int i, int smallest)
{
	t_heap_node	tmp;

	tmp = heap->nodes[i];
	heap->nodes[i] = heap->nodes[smallest];
	heap->nodes[smallest] = tmp;
	i = smallest;
	return (smallest);
}

int	heap_peek(t_heap *heap, int *out_coder_id)
{
	if (!heap || heap->size == 0)
		return (0);
	*out_coder_id = heap->nodes[0].coder_id;
	return (1);
}
