#include "codexion.h"

static bool init_coders_dongles(t_sim *sim)
{
    int i;

    i = 0;
    while (i < sim->config.number_of_coders)
    {
        sim->coders[i].id = i + 1;
        sim->coders[i].last_compile_start = 0;
        sim->coders[i].compile_count = 0;
        sim->coders[i].left_dongle = i;
        sim->coders[i].right_dongle = (i + 1) % sim->config.number_of_coders;
        sim->coders[i].sim = sim;
        sim->dongles[i].owner_id = -1;
        sim->dongles[i].available_at = 0;
        if (pthread_mutex_init(&sim->dongles[i].mutex, NULL) != 0)
        {
            while (i > 0)
                pthread_mutex_destroy(&sim->dongles[--i].mutex);
            return (false);
        }
        i++;
    }
    return (true);
}

static bool init_scheduler(t_sim *sim)
{
    sim->scheduler.heap = malloc(sizeof(t_request) * sim->config.number_of_coders);
    if (!sim->scheduler.heap)
        return (false);
    sim->scheduler.size = 0;
    sim->scheduler.capacity = sim->config.number_of_coders;
    if (pthread_mutex_init(&sim->scheduler.mutex, NULL) != 0)
        return (free(sim->scheduler.heap), false);
    if (pthread_cond_init(&sim->scheduler.cond, NULL) != 0)
        return (pthread_mutex_destroy(&sim->scheduler.mutex),
            free(sim->scheduler.heap), false);
    return (true);
}

static bool init_sim_mutexes(t_sim *sim)
{
    if (pthread_mutex_init(&sim->state_mutex, NULL) != 0)
        return (false);
    if (pthread_mutex_init(&sim->print_mutex, NULL) != 0)
        return (pthread_mutex_destroy(&sim->state_mutex), false);
    return (true);
}

bool    init_sim(t_sim *sim)
{
    sim->coders = malloc(sizeof(t_coder) * sim->config.number_of_coders);
    if (!sim->coders)
        return (false);
    sim->dongles = malloc(sizeof(t_dongle) * sim->config.number_of_coders);
    if (!sim->dongles)
        return (free(sim->coders), false);
    if (!init_coders_dongles(sim))
        return (free(sim->coders), free(sim->dongles), false);
    if (!init_scheduler(sim))
        return (clean_before_scheduler(sim), false);
    if (!init_sim_mutexes(sim))
        return (clean_before_sim_mutexes(sim), false);
    sim->start_time = get_time_ms();
    sim->stopped = false;
    return (true);
}