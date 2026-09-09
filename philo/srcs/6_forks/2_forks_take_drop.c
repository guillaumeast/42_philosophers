#include "forks.h"
#include "clock.h"
#include "mutex.h"
#include "helpers.h"
#include "logs.h"

static inline bool	forks_are_available(
						t_forks *forks,
						size_t meal_count,
						size_t left,
						size_t right)
{
	return (meal_count == forks->round
			&& forks->available[left] == true
			&& forks->available[right] == true);
}

static inline bool	forks_reserve(
						t_forks *forks,
						t_philo *philo,
						size_t left,
						size_t right)
{
	t_ms	elapsed;

	if (clock_get_elapsed(philo->run, &elapsed) == false)
		return (false);
	forks->available[left] = false;
	forks->available[right] = false;
	forks->meals_eaten++;
	if (forks->meals_eaten == philo->run->args.philo_count)
	{
		forks->round++;
		forks->meals_eaten = 0;
	}
	return (log_fork(philo, elapsed) && log_fork(philo, elapsed));
}

bool	forks_take(t_philo *philo, bool *out_taken)
{
	t_forks	*forks;
	size_t	left;
	size_t	right;
	bool	stopped;

	forks = &philo->run->forks;
	left = philo->id - 1;
	right = philo->id % forks->count;
	*out_taken = false;
	while (true)
	{
		if (clock_is_stopped(philo->run, &stopped) == false)
			return (false);
		if (stopped == true)
			return (true);
		if (mutex_lock(philo->run, &forks->mutex) == false)
			return (false);
		if (forks_are_available(forks, philo->meal_count, left, right) == true)
			return (forks_reserve(forks, philo, left, right),
					*out_taken = true,
					mutex_unlock(philo->run, &forks->mutex));
		if (mutex_unlock(philo->run, &forks->mutex) == false
			|| sleep_for_us(philo->run, SLEEP_DURATION_US) == false)
			return (false);
	}
}

bool	forks_drop(t_philo *philo)
{
	t_forks	*forks;

	forks = &philo->run->forks;
	if (mutex_lock(philo->run, &forks->mutex) == false)
		return (false);
	forks->available[philo->id - 1] = true;
	forks->available[philo->id % forks->count] = true;
	return (mutex_unlock(philo->run, &forks->mutex));
}
