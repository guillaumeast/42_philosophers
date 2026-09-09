#include "philo_priv.h"
#include "thread.h"
#include "mutex.h"
#include <stdlib.h>

void	philo_init(t_philo *philo)
{
	mutex_init(&philo->mutex);
	philo->id = UNSET_SIZE_T;
	thread_init(&philo->thread);
	philo->last_meal = UNSET_MS;
	philo->meal_count = UNSET_SIZE_T;
	philo->run = NULL;
}

bool	philo_load(t_run *run, size_t index)
{
	t_philo	*philo;

	philo = &run->philos[index];
	if (mutex_load(run, &philo->mutex, true) == false)
		return (false);
	philo->id = index + 1;
	philo->last_meal = 0;
	philo->meal_count = 0;
	philo->run = run;
	return (true);
}

void	philo_free(t_philo *philo)
{
	if (philo->id == UNSET_SIZE_T)
		return ;
	thread_free(philo->run, &philo->thread);
	mutex_free(philo->run, &philo->mutex, true);
	philo_init(philo);
}
