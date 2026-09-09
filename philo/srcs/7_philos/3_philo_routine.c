#include "philo_priv.h"
#include "logs.h"
#include "clock.h"
#include "mutex.h"
#include "helpers.h"
#include <unistd.h>

static inline bool	wait_for_start(t_run *run)
{
	bool	started;

	started = false;
	while (started == false)
	{
		if (clock_is_started(run, &started) == false)
			return (false);
	}
	return (true);
}

static inline bool	wait_for_death(t_philo *philo)
{
	t_ms	elapsed;
	bool	stopped;

	if (mutex_lock(philo->run, philo->forks[0]) == false)
		return (false);
	if (clock_get_elapsed(philo->run, &elapsed) == false)
		return ((void)mutex_unlock(philo->run, philo->forks[0]), false);
	if (log_fork(philo, elapsed) == false)
		return ((void)mutex_unlock(philo->run, philo->forks[0]), false);
	while (true)
	{
		if (clock_is_stopped(philo->run, &stopped) == false)
			return ((void)mutex_unlock(philo->run, philo->forks[0]), false);
		if (stopped == true)
			break ;
		if (sleep_for_us(philo->run, SLEEP_DURATION_US) == false)
			return ((void)mutex_unlock(philo->run, philo->forks[0]), false);
	}
	return (mutex_unlock(philo->run, philo->forks[0]));
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	bool	stopped;

	philo = arg;
	if (wait_for_start(philo->run) == false)
		return (NULL);
	if (philo->forks[1] == NULL)
		return ((void)wait_for_death(philo), NULL);
	while (true)
	{
		if (clock_is_stopped(philo->run, &stopped) == false || stopped == true)
			return (NULL);
		if (!philo_eat(philo) || !philo_sleep(philo) || !philo_think(philo))
			return (NULL);
	}
}
