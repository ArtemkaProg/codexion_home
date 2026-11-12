#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>

#define bool int
#define true 1
#define false 0

typedef enum e_scheduler
{
    FIFO,
    EDF,
    CUSTOM
}   t_scheduler_type;

typedef struct s_config
{
    int number_of_coders;
    int time_to_burnout;
    int time_to_compile;
    int time_to_debug;
    int time_to_refactor;
    int number_of_compiles_required;
    int dongle_cooldown;
    t_scheduler_type scheduler;
} t_config;

typedef struct s_dongle
{
    pthread_mutex_t mutex;
    int     owner_id;
    long    available_at;
    bool    is_available;
} t_dongle;

typedef struct s_coder
{
    int             id;
    pthread_t       thread;
    long            last_compile_start;
    int             compile_count;
    int             left_dongle;
    int             right_dongle;
    struct s_sim    *sim;
}   t_coder;

typedef struct s_request
{
    int     coder_id;
    long    arrival_time;
    long    deadline;
    long    sequence;
}   t_request;

typedef struct s_scheduler
{
    t_request       *heap;
    int             size;
    int             capacity;
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
}   t_scheduler;

typedef struct s_sim
{
    t_config        config;
    t_coder         *coders;
    t_dongle        *dongles;
    t_scheduler     scheduler;
    pthread_t       monitor;
    pthread_mutex_t state_mutex;
    pthread_mutex_t print_mutex;
    long            start_time;
    int             stopped;
}   t_sim;

int ft_strlen(const char *s);
char *ft_strdup(char *src);
bool ft_strcmp(const char *s1, const char *s2);
int ft_atoi(const char *str);
