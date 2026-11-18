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

static void give_dongles(t_sim *sim)
{
    int id;

    id = sim->scheduler.heap[0].coder_id;
    // left = sim->coders[id].left_dongle;
    // righ = sim->coders[id].right_dongle;
    pthread_mutex_lock(&sim->dongles[id].mutex);
    sim->dongles[coder->left_dongle].owner_id = coder->id;
    sim->dongles[coder->right_dongle].owner_id = coder->id;
    pthread_mutex_lock(&sim->dongles[id].mutex);
}

void process_requests(t_sim *sim)
{
    bool (*cmp)(t_request *, int);

    if (sim->config.scheduler == FIFO)
        cmp = fifo
    else if (sim->config.scheduler == EDF)
        cmp = edf
    else if (sim->config.scheduler == CUSTOM)
        // cmp = custom
    heapify_up(sim->scheduler.heap, sim->scheduler.size, cmp);
    pthread_cond_signal(&sim->scheduler.cond);
    give_dongles()
}