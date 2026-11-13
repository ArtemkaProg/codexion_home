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
                break;
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
    config->scheduler = (ft_strcmp(args[8], "fifo") == 0) ? FIFO : EDF;
    return (true);
}