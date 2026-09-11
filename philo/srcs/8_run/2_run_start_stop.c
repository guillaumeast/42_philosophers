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
	if (time_now(run, false, &now) == false
		|| clock_start(run, now) == false)
		return ((void)philos_stop(run, run->args.philo_count), false);
	return (run_monitor(run));
}

static bool	run_stop_safe(t_run *run, bool error, const char *message)
{
	run->clock.stop = true;
	run->clock.error |= error;
	if (error == true)
		return (log_error(run, message));
	return (logs_close(run));
}

bool	run_stop_locked(t_run *run, bool error, const char *message)
{
	return (run_stop_safe(run, error, message));
}

bool	run_stop(t_run *run, bool error, const char *message)
{
	bool	success;

	if (mutex_lock(run, &run->mutex) == false)
		return (false);
	success = run_stop_safe(run, error, message);
	return (mutex_unlock(run, &run->mutex) && success);
}
