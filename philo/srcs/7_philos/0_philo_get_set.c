#include "philos.h"
#include "philo_priv.h"
#include "mutex.h"

bool	philo_set_last_meal(t_philo *philo, t_ms last_meal)
{
	if (mutex_lock(philo->run, &philo->mutex) == false)
		return (false);
	philo->last_meal = last_meal;
	return (mutex_unlock(philo->run, &philo->mutex));
}

bool	philo_increment_meals(t_philo *philo)
{
	if (mutex_lock(philo->run, &philo->mutex) == false)
		return (false);
	philo->meal_count++;
	return (mutex_unlock(philo->run, &philo->mutex));
}

bool	philo_get_state(
			t_philo *philo,
			t_ms *last_meal,
			size_t *meal_count)
{
	if (mutex_lock(philo->run, &philo->mutex) == false)
		return (false);
	*last_meal = philo->last_meal;
	*meal_count = philo->meal_count;
	return (mutex_unlock(philo->run, &philo->mutex));
}
