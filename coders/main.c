#include "codexion.h"

static void join_coders(t_sim *sim)
{
    int i;

    i = 0;
    while (i < sim->config.number_of_coders)
    {
        pthread_join(sim->coders[i].thread, NULL);
        i++;
    }
}

static bool launch_coders(t_sim *sim)
{
    int i;

    i = 0;
    while (i < sim->config.number_of_coders)
    {
        if (pthread_create(&sim->coders[i].thread, NULL, coder_routine, &sim->coders[i]) != 0)
        {
            printf("Error: Failed to create thread for coder %d\n", i + 1);
            pthread_mutex_lock(&sim->state_mutex);
            sim->stopped = 1;
            pthread_mutex_unlock(&sim->state_mutex);
            while (i > 0)
                pthread_join(sim->coders[--i].thread, NULL);
            return (false);
        }
        i++;
    }
    if (pthread_create(&sim->scheduler_thread, NULL, scheduler_routine, sim) != 0)
    {
        printf("Error: Failed to create thread for scheduler\n");
        join_coders(sim);
        return (false);
    }
    // temp
    pthread_join(sim->scheduler_thread, NULL);
    join_coders(sim);
    return (true);
}

bool run_sim(t_sim *sim)
{
    launch_coders(sim);
    // int i = 0;
    // while (i < sim->scheduler.size)
    // {
    //     printf("Coder ID: %d, Arrival Time: %ld, Deadline: %ld, Sequence: %ld\n",
    //         sim->scheduler.heap[i].coder_id, sim->scheduler.heap[i].arrival_time, sim->scheduler.heap[i].deadline, sim->scheduler.heap[i].sequence);
    //     i++;
    // }

    return (true);
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