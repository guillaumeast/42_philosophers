#include "args.h"
#include "args_priv.h"
#include <limits.h>
#include <stdio.h>

#define OPERAND_MIN		4
#define OPERAND_MAX		5
#define OPT_ARG_NAME	"number of times each philosopher must eat"

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

static inline void	args_load_flag(t_args *args, int *argc, char ***argv)
{
	if (*argc >= 1
		&& (*argv)[0][0] == '-'
		&& (*argv)[0][1] == 'l'
		&& (*argv)[0][2] == '\0')
	{
		args->custom_logs = true;
		(*argc)--;
		(*argv)++;
	}
}

bool	args_load(t_args *args, int argc, char **argv)
{
	const char	*name;

	name = argv[0];
	argc--;
	argv++;
	args_load_flag(args, &argc, &argv);
	if (argc < OPERAND_MIN || argc > OPERAND_MAX)
	{
		(void)printf("%s: Invalid usage: Expected %i to %i operands, got %i\n",
				name, OPERAND_MIN, OPERAND_MAX, argc);
		return (false);
	}
	if (args_parse_size_t(argv[0], &args->philo_count, false) == false)
		return (args_err(args, name, argv[ + 0], "number of philosophers"));
	if (args_parse_ms(argv[1], &args->time_to_die) == false)
		return (args_err(args, name, argv[1], "time to die"));
	if (args_parse_ms(argv[2], &args->time_to_eat) == false)
		return (args_err(args, name, argv[2], "time to eat"));
	if (args_parse_ms(argv[3], &args->time_to_sleep) == false)
		return (args_err(args, name, argv[3], "time to sleep"));
	if (argc < OPERAND_MAX)
		return (true);
	if (args_parse_size_t(argv[4], &args->meals_count, true) == false)
		return (args_err(args, name, argv[4], OPT_ARG_NAME));
	args->meals_is_set = true;
	return (true);
}
