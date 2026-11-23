#include "codexion.h"

typedef bool (*t_cmp)(t_request *, t_request *);

static bool fifo(t_request *a, t_request *b)
{
    return (a->sequence < b->sequence);
}

static bool edf(t_request *a, t_request *b)
{
    if (a->deadline != b->deadline)
        return (a->deadline < b->deadline);
    return (a->sequence < b->sequence);
}

static t_cmp get_comparator(t_scheduler_type type)
{
    if (type == EDF)
        return (edf);
    return (fifo);
}

void heapify_up(t_request *heap, int size,
    bool (*cmp)(t_request *, t_request *))
{
    int i;
    int parent;
    t_request temp;

    i = size - 1;
    while (i > 0)
    {
        parent = (i - 1) / 2;
        if (!cmp(&heap[i], &heap[parent]))
            break ;
        temp = heap[i];
        heap[i] = heap[parent];
        heap[parent] = temp;
        i = parent;
    }
}

void heapify_down(t_request *heap, int size,
    bool (*cmp)(t_request *, t_request *))
{
    int i;
    int left;
    int right;
    int smallest;
    t_request temp;

    i = 0;
    while (1)
    {
        left = 2 * i + 1;
        right = 2 * i + 2;
        smallest = i;
        if (left < size && cmp(&heap[left], &heap[smallest]))
            smallest = left;
        if (right < size && cmp(&heap[right], &heap[smallest]))
            smallest = right;
        if (smallest == i)
            break ;
        temp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = temp;
        i = smallest;
    }
}

static bool give_dongles(t_sim *sim, t_request *request)
{
    t_coder *coder;
    t_dongle *left;
    t_dongle *right;
    long now;

    coder = &sim->coders[request->coder_id - 1];
    left = &sim->dongles[coder->left_dongle];
    right = &sim->dongles[coder->right_dongle];
    now = get_time_ms();

    if (left == right)
        return (false);
    if (left->owner_id != -1 || right->owner_id != -1)
        return (false);
    if (left->available_at > now || right->available_at > now)
        return (false);

    left->owner_id = coder->id;
    right->owner_id = coder->id;
    return (true);
}

void push_request(t_sim *sim, t_request request)
{
    t_scheduler *scheduler;

    scheduler = &sim->scheduler;
    scheduler->heap[scheduler->size] = request;
    scheduler->size++;
    heapify_up(scheduler->heap, scheduler->size,
        get_comparator(sim->config.scheduler));
    pthread_cond_signal(&scheduler->cond);
}

t_request pop_request(t_scheduler *scheduler,
    bool (*cmp)(t_request *, t_request *))
{
    t_request request;

    request = scheduler->heap[0];
    scheduler->size--;
    if (scheduler->size > 0)
    {
        scheduler->heap[0] = scheduler->heap[scheduler->size];
        heapify_down(scheduler->heap, scheduler->size, cmp);
    }
    return (request);
}

bool process_requests(t_sim *sim)
{
    t_request request;

    if (sim->scheduler.size == 0)
        return (false);

    request = sim->scheduler.heap[0];

    if (!give_dongles(sim, &request))
        return (false);

    pop_request(&sim->scheduler,
        get_comparator(sim->config.scheduler));

    return (true);
}

void *scheduler_routine(void *arg)
{
    t_sim *sim;

    sim = (t_sim *)arg;
    pthread_mutex_lock(&sim->scheduler.mutex);

    while (!sim->stopped)
    {
        if (sim->scheduler.size == 0 ||
            !process_requests(sim))
        {
            pthread_cond_wait(&sim->scheduler.cond,
                &sim->scheduler.mutex);
        }
    }

    pthread_mutex_unlock(&sim->scheduler.mutex);
    return (NULL);
}