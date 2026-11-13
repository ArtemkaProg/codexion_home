#include "codexion.h"

bool    init_sim(t_sim *sim)
{
    int i;

    sim->coders = malloc(sizeof(t_coder) * sim->config.number_of_coders);
    if (!sim->coders)
        return (false);
    sim->dongles = malloc(sizeof(t_dongle) * sim->config.number_of_coders);
    if (!sim->dongles)
    {
        free(sim->coders);
        return (false);
    }
    i = 0;
    while (i < sim->config.number_of_coders)
    {
        sim->coders[i].id = i;
        sim->coders[i].last_compile_start = 0;
        sim->coders[i].compile_count = 0;
        sim->coders[i].left_dongle = i;
        sim->coders[i].right_dongle = (i + 1) % sim->config.number_of_coders;
        sim->coders[i].sim = sim;
        sim->dongles[i].owner_id = -1;
        sim->dongles[i].available_at = 0;
        pthread_mutex_init(&sim->dongles[i].mutex, NULL);
        i++;
    }
    return (true);
}