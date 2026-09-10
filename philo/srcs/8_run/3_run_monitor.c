#include "run_priv.h"
#include "philos.h"
#include "clock.h"
#include "run.h"
#include "helpers.h"
#include "logs.h"
#include "mutex.h"
#include <unistd.h>

static inline bool	run_monitor_all(t_run *run)
{
	bool	stopped;
	t_ms	elapsed;
	bool	ended;
	size_t	i;
	t_philo	*philo;

	if (clock_is_stopped(run, &stopped) == false)
		return (false);
	if (stopped == true)
		return (true);
	if (mutex_lock(run, &run->mutex) == false)
		return (false);
	if (clock_get_elapsed(run, &elapsed) == false)
		return ((void)mutex_unlock(run, &run->mutex), false);
	ended = run->args.meals_is_set;
	i = 0;
	while (i < run->philos.count)
	{
		philo = &run->philos.list[i++];
		if (ended == true && philo->meal_count < run->args.meals_count)
			ended = false;
		if (philo->last_meal <= elapsed - run->args.time_to_die)
			return (mutex_unlock(run, &run->mutex) && log_death(philo, elapsed));
	}
	if (mutex_unlock(run, &run->mutex) == false)
		return (false);
	return (ended == false || run_stop(run, false, NULL));
}

bool	run_monitor(t_run *run)
{
	bool	error;
	bool	stopped;

	error = false;
	while (true)
	{
		if (!run_monitor_all(run) || !clock_is_stopped(run, &stopped))
		{
			error = true;
			break ;
		}
		if (stopped == true)
			break ;
		if (sleep_for_us(run, SLEEP_DURATION_US) == false)
		{
			error = true;
			break ;
		}
	}
	if (philos_stop(run, run->args.philo_count) == false)
		return (false);
	return (error == false && clock_is_error(run) == false);
}
