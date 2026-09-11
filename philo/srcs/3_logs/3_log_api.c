#include "logs.h"
#include "log_priv.h"

bool	log_fork(t_philo *philo, t_ms elapsed)
{
	return (log_priv(philo, elapsed, false, LOG_FORK));
}

bool	log_eat(t_philo *philo)
{
	return (log_priv(philo, philo->last_meal, false, LOG_EAT));
}

bool	log_sleep(t_philo *philo, t_ms elapsed)
{
	return (log_priv(philo, elapsed, false, LOG_SLEEP));
}

bool	log_think(t_philo *philo, t_ms elapsed)
{
	return (log_priv(philo, elapsed, false, LOG_THINK));
}

bool	log_death_locked(t_philo *philo, t_ms elapsed)
{
	return (log_priv(philo, elapsed, true, LOG_DEATH));
}
