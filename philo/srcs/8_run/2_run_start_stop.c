#include "run.h"
#include "run_priv.h"
#include "philos.h"
#include "clock.h"
#include "helpers.h"
#include "logs.h"
#include "mutex.h"

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

bool	run_stop(t_run *run, bool locked, bool error, const char *opt_message)
{
	if (!locked)
		error = !mutex_lock(run, &run->mutex) || error;
	run->clock.stop = true;
	if (error == true)
		log_error(run, opt_message);	// ! deadlock
	if (!locked && !mutex_unlock(run, &run->mutex))
		return (false);
	return (!error);
}
