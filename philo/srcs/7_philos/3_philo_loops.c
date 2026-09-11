#include "philo_priv.h"
#include "clock.h"
#include "forks.h"
#include "helpers.h"
#include "logs.h"

bool	philo_eat(t_philo *philo)
{
	t_run	*run;
	t_ms	target;

	run = philo->run;
	if (forks_take(philo) == false)
		return (false);
	return (log_eat(philo)
		&& time_try_add(run, philo->last_meal, run->args.time_to_eat, &target)
		&& sleep_until(run, target)
		&& forks_drop(philo));
}

bool	philo_sleep(t_philo *philo)
{
	t_run	*run;
	t_ms	elapsed;
	t_ms	target;

	run = philo->run;
	return (clock_get_elapsed(run, &elapsed)
		&& log_sleep(philo, elapsed)
		&& time_try_add(run, elapsed, run->args.time_to_sleep, &target)
		&& sleep_until(run, target));
}

bool	philo_think(t_philo *philo)
{
	t_run	*run;
	t_ms	start;

	run = philo->run;
	return (clock_get_elapsed(run, &start) && log_think(philo, start));
}
