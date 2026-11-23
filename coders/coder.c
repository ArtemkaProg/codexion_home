#include "codexion.h"

static bool simulation_stopped(t_sim *sim)
{
    bool stopped;

    pthread_mutex_lock(&sim->scheduler.mutex);
    stopped = sim->stopped;
    pthread_mutex_unlock(&sim->scheduler.mutex);
    return (stopped);
}

bool acquire_dongles(t_coder *coder)
{
    t_sim *sim;
    t_request request;

    sim = coder->sim;
    pthread_mutex_lock(&sim->scheduler.mutex);

    if (sim->stopped ||
        sim->scheduler.size >= sim->scheduler.capacity)
    {
        pthread_mutex_unlock(&sim->scheduler.mutex);
        return (false);
    }

    request.coder_id = coder->id;
    request.arrival_time = get_time_ms();

    if (coder->last_compile_start == 0)
        request.deadline = sim->start_time
            + sim->config.time_to_burnout;
    else
        request.deadline = coder->last_compile_start
            + sim->config.time_to_burnout;

    request.sequence = sim->scheduler.next_sequence++;

    push_request(sim, request);

    pthread_mutex_unlock(&sim->scheduler.mutex);

    return (false); // TEMP: очікування видачі ще не реалізовано
}

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