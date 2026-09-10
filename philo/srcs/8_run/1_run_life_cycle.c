#include "run.h"
#include "logs.h"
#include "clock.h"
#include "forks.h"
#include "philos.h"
#include "args.h"
#include "mutex.h"

void	run_init(t_run *run)
{
	logs_init(&run->logs);
	mutex_init(&run->mutex);
	args_init(&run->args);
	clock_init(&run->clock);
	forks_init(&run->forks);
	philos_init(&run->philos);
}

bool	run_load(t_run *run, int argc, char **argv)
{
	if (logs_load(run, argv[0]) == false)
		return (false);
	if (mutex_load(run, &run->mutex, true) == false
		|| args_load(&run->args, argc, argv) == false
		|| clock_load(run) == false
		|| forks_load(run) == false
		|| philos_load(run) == false)
		return (run_free(run), false);
	return (true);
}

void	run_free(t_run *run)
{
	philos_free(run, run->args.philo_count);
	forks_free(run);
	clock_free(run);
	args_free(&run->args);
	mutex_free(run, &run->mutex, true);
	logs_free(run);
}
