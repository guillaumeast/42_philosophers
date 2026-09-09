#include "forks.h"
#include "clock.h"
#include "mutex.h"
#include "philos.h"
#include "helpers.h"
#include "logs.h"
#include <limits.h>
#include <stdint.h>

static inline bool	forks_has_priority(t_philo *philo, bool *out)
{
	size_t	i;
	size_t	meal_count;
	t_ms	last_meal;
	t_ms	time_min;

	i = 0;
	time_min = LLONG_MAX;
	while (i < philo->run->args.philo_count)
	{
		if (!philo_get_state(&philo->run->philos[i], &last_meal, &meal_count))
			return (false);
		if (last_meal < time_min)
			time_min = last_meal;
		i++;
	}
	*out = philo->last_meal == time_min;
	return (true);
}

static inline bool	forks_take_and_log(t_forks *forks, t_philo *philo)
{
	t_ms	elapsed;

	if (clock_get_elapsed(philo->run, &elapsed) == false)
		return (false);
	forks->available[philo->left_fork] = false;
	forks->available[philo->right_fork] = false;
	forks->meals_eaten++;
	if (forks->meals_eaten == philo->run->args.philo_count)
	{
		forks->round++;
		forks->meals_eaten = 0;
	}
	return (log_fork(philo, elapsed) && log_fork(philo, elapsed));
}

static inline bool	forks_try_take(t_forks *forks, t_philo *philo, bool *taken)
{
	bool	has_priority;

	*taken = false;
	if (mutex_lock(philo->run, &forks->mutex) == false)
		return (false);
	if (forks_has_priority(philo, &has_priority) == false)
		return ((void)mutex_unlock(philo->run, &forks->mutex), false);
	if (has_priority == false
		|| forks->available[philo->left_fork] == false
		|| forks->available[philo->right_fork] == false)
		return (mutex_unlock(philo->run, &forks->mutex));
	if (forks_take_and_log(forks, philo) == false)
		return ((void)mutex_unlock(philo->run, &forks->mutex), false);
	*taken = true;
	return (mutex_unlock(philo->run, &forks->mutex));
}

bool	forks_take(t_philo *philo, bool *out_taken)
{
	t_forks	*forks;
	bool	stopped;

	forks = &philo->run->forks;
	*out_taken = false;
	while (true)
	{
		if (clock_is_stopped(philo->run, &stopped) == false)
			return (false);
		if (stopped == true)
			return (true);
		if (forks_try_take(forks, philo, out_taken) == false)
			return (false);
		if (*out_taken == true)
			return (true);
		if (sleep_for_us(philo->run, SLEEP_DURATION_US) == false)
			return (false);
	}
}

bool	forks_drop(t_philo *philo)
{
	t_forks	*forks;

	forks = &philo->run->forks;
	if (mutex_lock(philo->run, &forks->mutex) == false)
		return (false);
	forks->available[philo->left_fork] = true;
	forks->available[philo->right_fork] = true;
	return (mutex_unlock(philo->run, &forks->mutex));
}
