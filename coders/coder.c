#include "codexion.h"

static bool simulation_stopped(t_sim *sim)
{
    bool stopped;

    pthread_mutex_lock(&sim->state_mutex);
    stopped = sim->stopped;
    pthread_mutex_unlock(&sim->state_mutex);
    return (stopped);
}

static void send_request(t_coder *coder)
{
    t_sim *sim;
    int i;

    sim = coder->sim;
    i = sim->scheduler.size;
    sim->scheduler.heap[i].coder_id = coder->id;
    sim->scheduler.heap[i].arrival_time = get_time_ms();
    if (coder->last_compile_start == 0)
        sim->scheduler.heap[i].deadline = sim->start_time
            + sim->config.time_to_burnout;
    else
        sim->scheduler.heap[i].deadline = coder->last_compile_start
            + sim->config.time_to_burnout;
    sim->scheduler.heap[i].sequence = sim->scheduler.next_sequence++;
    sim->scheduler.size++;
}

bool acquire_dongles(t_coder *coder)
{
    t_sim *sim;
    
    sim = coder->sim;
    
    pthread_mutex_lock(&sim->scheduler.mutex);
    if (sim->scheduler.size >= sim->scheduler.capacity)
        return (pthread_mutex_unlock(&sim->scheduler.mutex), false);
    send_request(coder);
    pthread_cond_signal(&sim->scheduler.cond, &sim->scheduler.mutex);
    // process_requests(coder->sim)
    pthread_mutex_unlock(&sim->scheduler.mutex);
    return (false); // temp return
}
// sim->dongles[coder->left_dongle].owner_id = coder->id;
// sim->dongles[coder->right_dongle].owner_id = coder->id;

void *coder_routine(void *arg)
{
    t_coder *coder;

    coder = (t_coder *)arg;
    while (!simulation_stopped(coder->sim))
    {
        if (!acquire_dongles(coder))
            break ;
        // compile(coder);
        // release_dongles(coder);
        if (simulation_stopped(coder->sim))
            break ;
        // debug(coder);
        // refactor(coder);
    }
    return (NULL);
}