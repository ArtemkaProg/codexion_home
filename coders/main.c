#include "codexion.h"

static void join_coders(t_sim *sim, int count)
{
    int i;

    i = 0;
    while (i < count)
    {
        pthread_join(sim->coders[i].thread, NULL);
        i++;
    }
}

static bool launch_coders(t_sim *sim, int *created)
{
    int i;

    *created = 0;
    i = 0;
    while (i < sim->config.number_of_coders)
    {
        if (pthread_create(&sim->coders[i].thread,
                NULL, coder_routine, &sim->coders[i]) != 0)
        {
            printf("Error: Failed to create coder %d\n", i + 1);
            return (false);
        }
        (*created)++;
        i++;
    }
    return (true);
}

bool run_sim(t_sim *sim)
{
    int created;
    bool success;

    if (pthread_create(&sim->scheduler_thread,
            NULL, scheduler_routine, sim) != 0)
        return (false);

    success = launch_coders(sim, &created);

    // TEMP: кодери поки завершуються після надсилання запиту.
    join_coders(sim, created);

    pthread_mutex_lock(&sim->scheduler.mutex);
    sim->stopped = true;
    pthread_cond_broadcast(&sim->scheduler.cond);
    pthread_mutex_unlock(&sim->scheduler.mutex);

    pthread_join(sim->scheduler_thread, NULL);
    return (success);
}

int main(int argc, char **args)
{
    t_sim   sim;

    if (!parse_config(argc, args, &sim.config))
        return (1);
    if (!init_sim(&sim))
    {
        printf("Error: Failed to initialize simulation\n");
        return (1);
    }
    run_sim(&sim);
    printf("Number of coders: %d\n", sim.config.number_of_coders);
    printf("Start time: %ld ms\n", get_time_ms() - sim.start_time);
    clean_sim(&sim);
    return (0);
}