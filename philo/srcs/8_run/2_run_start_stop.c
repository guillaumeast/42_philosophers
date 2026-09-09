#include "run.h"
#include "run_priv.h"
#include "philos.h"
#include "clock.h"
#include "helpers.h"
#include "logs.h"

bool	run_start(t_run *run)
{
	t_ms	now;

	if (run->args.meals_is_set == true && run->args.meals_count == 0)
		return (true);
	if (philos_start(run) == false)
		return (false);
	if (time_now(run, &now) == false
		|| clock_start(run, now) == false)
		return ((void)philos_stop(run, run->args.philo_count), false);
	return (run_monitor(run));
}

bool	run_stop(t_run *run, bool error, const char *opt_message)
{
	bool	success;

	success = clock_stop(run, error);
	if (error == true)
		log_error(run, opt_message);
	return (success == true && error == false);
}
