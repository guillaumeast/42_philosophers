#include "philos.h"
#include "philo_priv.h"
#include "clock.h"
#include "mutex.h"
#include "helpers.h"
#include "logs.h"

static inline bool	philo_drop_forks(t_philo *philo)
{
	bool	success;

	success = mutex_unlock(philo->run, philo->forks[0]);
	return (mutex_unlock(philo->run, philo->forks[1]) && success == true);
}

static inline bool	philo_take_forks(t_run *run, t_philo *philo)
{
	bool	success;
	t_ms	elapsed;

	if (mutex_lock(run, philo->forks[0]) == false)
		return (false);
	if (clock_get_elapsed(run, &elapsed) == false
		|| log_fork(philo, elapsed) == false
		|| mutex_lock(run, philo->forks[1]) == false)
		return ((void)mutex_unlock(run, philo->forks[0]), false);
	success = clock_get_elapsed(run, &elapsed) && log_fork(philo, elapsed);
	return (philo_drop_forks(philo) && success == true);
}

bool	philo_eat(t_philo *philo)
{
	t_run	*run;
	bool	success;
	t_ms	elapsed;

	run = philo->run;
	if (philo_take_forks(run, philo) == false)
		return (false);
	success = clock_get_elapsed(run, &elapsed)
		&& philo_set_last_meal(philo, elapsed)
		&& log_eat(philo, elapsed)
		&& sleep_for(run, run->args.time_to_eat)
		&& philo_increment_meals(philo);
	return (philo_drop_forks(philo) && success == true);
}

bool	philo_sleep(t_philo *philo)
{
	t_run	*run;
	t_ms	elapsed;

	run = philo->run;
	return (clock_get_elapsed(run, &elapsed)
		&& log_sleep(philo, elapsed)
		&& sleep_for(run, run->args.time_to_sleep));
}

bool	philo_think(t_philo *philo)
{
	t_run	*run;
	t_ms	start;

	run = philo->run;
	return (clock_get_elapsed(run, &start) && log_think(philo, start));
}
