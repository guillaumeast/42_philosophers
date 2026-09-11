#include "run_priv.h"
#include "philos.h"
#include "clock.h"
#include "run.h"
#include "helpers.h"
#include "logs.h"
#include "mutex.h"
#include <unistd.h>

static inline bool	is_dead(t_run *run, t_philo *philo, t_ms now, bool *out)
{
	if (philo->last_meal + run->args.time_to_die <= now)
	{
		*out = true;
		run->clock.stop = true;
		return (log_death(philo, now));
	}
	*out = false;
	return (true);
}

static inline bool	run_monitor_all_safe(t_run *run, bool *out_ended)
{
	t_ms	now;
	t_philo	*philo;
	bool	dead;
	size_t	i;

	if (run->clock.stop == true)
	{
		*out_ended = true;
		return (true);
	}
	if (clock_get_elapsed(run, &now) == false)
		return (false);
	*out_ended = run->args.meals_is_set;
	i = 0;
	while (i < run->philos.count)
	{
		philo = &run->philos.list[i++];
		if (is_dead(run, philo, now, &dead) == false)
			return (false);
		if (dead == true)
			return (*out_ended = true);
		*out_ended = *out_ended && philo->meal_count >= run->args.meals_count;
	}
	return (*out_ended == false || run_stop(run, false, NULL));
}

static inline bool	run_monitor_all(t_run *run, bool *out_ended)
{
	bool	success;

	if (mutex_lock(run, &run->mutex) == false)
		return (false);
	success = run_monitor_all_safe(run, out_ended);
	return (mutex_unlock(run, &run->mutex) && success);
}

bool	run_monitor(t_run *run)
{
	bool	ended;

	while (true)
	{
		if (run_monitor_all(run, &ended) == false)
			return ((void)philos_stop(run, run->args.philo_count), false);
		if (ended == true)
			break ;
		if (sleep_for_us(run, SLEEP_DURATION_US) == false)
			return ((void)philos_stop(run, run->args.philo_count), false);
	}
	return (philos_stop(run, run->args.philo_count));
}
