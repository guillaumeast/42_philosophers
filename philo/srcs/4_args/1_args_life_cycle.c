#include "args.h"

void	args_init(t_args *args)
{
	args->philo_count = UNSET_SIZE_T;
	args->time_to_die = UNSET_MS;
	args->time_to_eat = UNSET_MS;
	args->time_to_sleep = UNSET_MS;
	args->meals_is_set = false;
	args->meals_count = UNSET_SIZE_T;
	args->custom_logs = false;
}

void	args_free(t_args *args)
{
	args_init(args);
}
