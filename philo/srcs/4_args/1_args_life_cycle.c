#include "args.h"
#include "args_priv.h"
#include <limits.h>
#include <stdio.h>

#define ARGC_MIN		5
#define ARGC_MAX		6
#define OPT_ARG_NAME	"number of times each philosopher must eat"

void	args_init(t_args *args)
{
	args->philo_count = UNSET_SIZE_T;
	args->time_to_die = UNSET_MS;
	args->time_to_eat = UNSET_MS;
	args->time_to_sleep = UNSET_MS;
	args->meals_is_set = false;
	args->meals_count = UNSET_SIZE_T;
}

static inline bool	args_err(
						t_args *args,
						const char *program_name,
						const char *arg_value,
						const char *arg_name)
{
	(void)printf("%s: error: '%s': invalid %s.\n",
		program_name, arg_value, arg_name);
	args_free(args);
	return (false);
}

bool	args_load(t_args *args, int argc, char **argv)
{
	if (argc < ARGC_MIN || argc > ARGC_MAX)
	{
		(void)printf("%s: Invalid usage: Expected %i to %i arguments, got %i\n",
				argv[0], ARGC_MIN, ARGC_MAX, argc);
		return (false);
	}
	if (args_parse_size_t(argv[1], &args->philo_count, false) == false)
		return (args_err(args, argv[0], argv[1], "number of philosophers"));
	if (args_parse_ms(argv[2], &args->time_to_die) == false)
		return (args_err(args, argv[0], argv[2], "time to die"));
	if (args_parse_ms(argv[3], &args->time_to_eat) == false)
		return (args_err(args, argv[0], argv[3], "time to eat"));
	if (args_parse_ms(argv[4], &args->time_to_sleep) == false)
		return (args_err(args, argv[0], argv[4], "time to sleep"));
	if (argc < 6)
		return (true);
	if (args_parse_size_t(argv[5], &args->meals_count, true) == false)
		return (args_err(args, argv[0], argv[5], OPT_ARG_NAME));
	args->meals_is_set = true;
	return (true);
}

void	args_free(t_args *args)
{
	args_init(args);
}
