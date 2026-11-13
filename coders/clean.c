#include "codexion.h"

void clean_sim(t_sim *sim)
{
    int i;

    i = 0;
    while (i < sim->config.number_of_coders)
    {
        pthread_mutex_destroy(&sim->dongles[i].mutex);
        i++;
    }
    free(sim->coders);
    free(sim->dongles);
}