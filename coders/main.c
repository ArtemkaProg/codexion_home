#include "codexion.h"

// void run_sim(t_sim *sim)

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
    printf("Number of coders: %d\n", sim.config.number_of_coders);
    return (0);
}