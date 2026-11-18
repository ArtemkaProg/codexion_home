#include "codexion.h"

static bool fifo(t_request *heap, int i)
{
    int parent;

    parent = (i - 1) / 2;
    return (heap[i].sequence < heap[parent].sequence);
}

static bool edf(t_request *heap, int i)
{
    int parent;

    parent = (i - 1) / 2;
    return (heap[i].deadline < heap[parent].deadline);
}

void heapify_up(t_request *heap, int size, bool (*cmp)(t_request *, int))
{
    int i;
    int parent;
    t_request temp;

    if (!heap)
        return ;
    i = size - 1;
    while (i > 0)
    {
        if (!cmp(heap, i))
            break ;
        parent = (i - 1) / 2;
        temp = heap[i];
        heap[i] = heap[parent];
        heap[parent] = temp;
        i = parent;
    }
}

void process_requests(t_sim *sim)
{
    if sim->config.scheduler == FIFO
        heapify_up(sim->scheduler.heap, sim->scheduler.size, fifo);
    else if sim->config.scheduler == EDF
        heapify_up(sim->scheduler.heap, sim->scheduler.size, edf);
    else if sim->config.scheduler == CUSTOM
        // heapify_up(sim->scheduler.heap, sim->scheduler.size, custom);
        ;
    else
        return ; // unknown scheduler
    
}