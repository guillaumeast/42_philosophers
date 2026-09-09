#include "run_priv.h"
#include "philos.h"
#include "clock.h"
#include "run.h"
#include "helpers.h"
#include "logs.h"
#include <unistd.h>

static inline bool	monitor_one(
						t_philo *philo,
						bool *meals_completed,
						t_ms elapsed,
						t_ms deadline)
{
	t_ms	last_meal;
	size_t	meal_count;

	if (philo_get_state(philo, &last_meal, &meal_count) == false)
		return (false);
	if (*meals_completed == true && philo->run->args.meals_count > meal_count)
		*meals_completed = false;
	if (last_meal <= deadline)
		return (log_death(philo, elapsed));
	return (true);
}

static inline bool	monitor_all(t_run *run)
{
	t_ms	elapsed;
	t_ms	deadline;
	bool	meals_completed;
	bool	stopped;
	size_t	i;

	if (clock_get_elapsed(run, &elapsed) == false)
		return (false);
	deadline = elapsed - run->args.time_to_die;
	meals_completed = run->args.meals_is_set;
	i = 0;
	while (i < run->args.philo_count)
	{
		if (clock_is_stopped(run, &stopped) == false)
			return (false);
		if (stopped == true)
			return (true);
		if (!monitor_one(&run->philos[i], &meals_completed, elapsed, deadline))
			return (false);
		i++;
	}
	return (meals_completed == false || run_stop(run, false, NULL));
}

bool	run_monitor(t_run *run)
{
	bool	error;
	bool	stopped;

	error = false;
	while (true)
	{
		if (!monitor_all(run) || !clock_is_stopped(run, &stopped))
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
