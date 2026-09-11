#include "philo_priv.h"
#include "logs.h"
#include "clock.h"
#include "helpers.h"

static inline bool	wait_for_start(t_philo *philo)
{
	bool	started;

	started = false;
	while (started == false)
	{
		if (clock_is_started(philo->run, &started) == false)
			return (false);
	}
	return (true);
}

static inline bool	wait_for_death(t_philo *philo)
{
	t_ms	elapsed;
	bool	stopped;

	if (clock_get_elapsed(philo->run, &elapsed) == false)
		return (false);
	if (log_fork(philo, elapsed) == false)
		return (false);
	while (true)
	{
		if (clock_is_stopped(philo->run, false, &stopped) == false)
			return (false);
		if (stopped == true)
			break ;
		if (sleep_for_us(philo->run, SLEEP_DURATION_US) == false)
			return (false);
	}
	return (true);
}

# include <stdio.h>
void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = arg;
	if (wait_for_start(philo) == false)
		return (NULL);
	if (philo->run->args.philo_count == 1)
		return ((void)wait_for_death(philo), NULL);
	while (true)
	{
		if (!philo_eat(philo) || !philo_sleep(philo) || !philo_think(philo))
			break ;
	}
	// tmp
	t_ms now;
	if (clock_get_elapsed(philo->run, &now))
		(void)log_stop(philo, now);
	return (NULL);
}
