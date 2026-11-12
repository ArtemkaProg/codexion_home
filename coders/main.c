#include "codexion.h"

static bool parse(int argc, char **argv)
{
    int i;
    int j;
    bool valid;

    valid = true;
    i = 1;
    while (i < argc - 1)
    {
        j = 0;
        while (argv[i][j])
        {
            if (!isdigit((unsigned char)argv[i][j]))
            {
                printf("Error: Invalid argument - %s\n", argv[i]);
                valid = false;
                i++;
                continue;
            }
            j++;
        }
        i++;
    }
    if (ft_strcmp(argv[8], "fifo") != 0 && ft_strcmp(argv[8], "edf") != 0)
    {
        printf("Error: Invalid scheduler - %s\n", argv[8]);
        valid = false;
    }
    return valid;
}

bool parse_config(int argc, char **args, t_config *config)
{
    if (argc != 9)
    {
        printf("Usage: <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <scheduler>\n");
        return (false);
    }
    if (!parse(argc, args))
        return (false);
    config->number_of_coders = ft_atoi(args[1]);
    config->time_to_burnout = ft_atoi(args[2]);
    config->time_to_compile = ft_atoi(args[3]);
    config->time_to_debug = ft_atoi(args[4]);
    config->time_to_refactor = ft_atoi(args[5]);
    config->number_of_compiles_required = ft_atoi(args[6]);
    config->dongle_cooldown = ft_atoi(args[7]);
    return (true);
}

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
        sim->dongles[i].is_available = true;
        pthread_mutex_init(&sim->dongles[i].mutex, NULL);
        i++;
    }
    return (true);
}

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