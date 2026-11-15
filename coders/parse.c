#include "codexion.h"

static bool    parse_numbers(char **args, t_config *cfg)
{
    int *fields[7];
    int i;

    fields[0] = &cfg->number_of_coders;
    fields[1] = &cfg->time_to_burnout;
    fields[2] = &cfg->time_to_compile;
    fields[3] = &cfg->time_to_debug;
    fields[4] = &cfg->time_to_refactor;
    fields[5] = &cfg->number_of_compiles_required;
    fields[6] = &cfg->dongle_cooldown;
    i = 0;
    while (i < 7)
    {
        if (!ft_atoi_safe(args[i + 1], fields[i]))
            return (false);
        if (*fields[i] <= 0 && i != 6)
            return (false);
        i++;
    }
    return (true);
}

bool    parse_config(int argc, char **args, t_config *config)
{
    if (argc != 9)
        return (printf("Error: Invalid argument count\n"), false);
    if (!parse_numbers(args, config))
        return (printf("Error: Invalid numeric argument\n"), false);
    if (ft_strcmp(args[8], "fifo") == 0)
        config->scheduler = FIFO;
    else if (ft_strcmp(args[8], "edf") == 0)
        config->scheduler = EDF;
    else
        return (printf("Error: Invalid scheduler\n"), false);
    return (true);
}