#include "codexion.h"

void clean_sim(t_sim *sim)
{
    pthread_mutex_destroy(&sim->state_mutex);
    pthread_mutex_destroy(&sim->print_mutex);
    clean_scheduler(sim);
    clean_before_scheduler(sim);
}

void free_dongles_mutexes(t_sim *sim)
{
    int i;

    i = 0;
    while (i < sim->config.number_of_coders)
    {
        pthread_mutex_destroy(&sim->dongles[i].mutex);
        i++;
    }
}

void clean_before_scheduler(t_sim *sim)
{
    free_dongles_mutexes(sim);
    free(sim->coders);
    free(sim->dongles);
}

void clean_scheduler(t_sim *sim)
{
    free(sim->scheduler.heap);
    pthread_mutex_destroy(&sim->scheduler.mutex);
    pthread_cond_destroy(&sim->scheduler.cond);
}

void clean_before_sim_mutexes(t_sim *sim)
{
    clean_before_scheduler(sim);
    clean_scheduler(sim);
}