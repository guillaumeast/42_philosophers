#include "forks.h"
#include "clock.h"
#include "mutex.h"
#include "helpers.h"
#include <limits.h>
#include <stdint.h>

static inline bool	forks_are_available(t_philo *philo)
{
	return (philo->last_meal <= philo->philo_left->last_meal
		&& philo->last_meal <= philo->philo_right->last_meal
		&& *philo->fork_left_is_available == true
		&& *philo->fork_right_is_available == true);
}

static inline bool	forks_try_take(t_philo *philo, bool *taken)
{
	*taken = false;
	if (mutex_lock(philo->run, &philo->run->mutex) == false)
		return (false);
	if (forks_are_available(philo) == false)
		return (mutex_unlock(philo->run, &philo->run->mutex));
	*philo->fork_left_is_available = false;
	*philo->fork_right_is_available = false;
	*taken = true;
	if (clock_get_elapsed(philo->run, &philo->last_meal) == false)
		return ((void)mutex_unlock(philo->run, &philo->run->mutex), false);
	return (mutex_unlock(philo->run, &philo->run->mutex));
}

bool	forks_take(t_philo *philo, bool *out_taken)
{
	bool	stopped;

	*out_taken = false;
	while (true)
	{
		if (clock_is_stopped(philo->run, &stopped) == false)
			return (false);
		if (stopped == true)
			return (true);
		if (forks_try_take(philo, out_taken) == false)
			return (false);
		if (*out_taken == true)
			return (true);
		if (sleep_for_us(philo->run, SLEEP_DURATION_US) == false)
			return (false);
	}
}

bool	forks_drop(t_philo *philo)
{
	if (mutex_lock(philo->run, &philo->run->mutex) == false)
		return (false);
	*philo->fork_left_is_available = true;
	*philo->fork_right_is_available = true;
	philo->meal_count++;
	return (mutex_unlock(philo->run, &philo->run->mutex));
}
