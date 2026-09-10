#include "philo_priv.h"
#include "thread.h"
#include <stdlib.h>

void	philo_init(t_philo *philo)
{
	philo->id = UNSET_SIZE_T;
	thread_init(&philo->thread);
	philo->last_meal = UNSET_MS;
	philo->meal_count = UNSET_SIZE_T;
	philo->fork_left_is_available = NULL;
	philo->fork_right_is_available = NULL;
	philo->philo_left = NULL;
	philo->philo_right = NULL;
	philo->run = NULL;
}

bool	philo_load(t_run *run, size_t index)
{
	t_philos	*philos;
	t_philo		*philo;
	size_t		right_fork_index;

	philos = &run->philos;
	philo = &philos->list[index];
	right_fork_index = (index + 1) % philos->count;
	philo->id = index + 1;
	philo->last_meal = 0;
	philo->meal_count = 0;
	philo->fork_left_is_available = &run->forks.available[index];
	philo->fork_right_is_available = &run->forks.available[right_fork_index];
	if (index == 0)
		philo->philo_left = &philos->list[philos->count - 1];
	else
		philo->philo_left = &philos->list[index - 1];
	philo->philo_right = &philos->list[(index + 1) % philos->count];
	philo->run = run;
	return (true);
}

void	philo_free(t_philo *philo)
{
	thread_free(philo->run, &philo->thread);
	philo_init(philo);
}
