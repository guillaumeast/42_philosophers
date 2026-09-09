#include "philo_priv.h"
#include "thread.h"
#include "mutex.h"
#include <stdlib.h>

void	philo_init(t_philo *philo)
{
	philo->id = UNSET_SIZE_T;
	thread_init(&philo->thread);
	philo->last_meal = UNSET_MS;
	philo->meal_count = UNSET_SIZE_T;
	philo->forks[0] = NULL;
	philo->forks[1] = NULL;
	philo->run = NULL;
}

static inline void	philo_set_forks(
						t_philo *philo,
						size_t index,
						t_mutex *forks,
						size_t count)
{
	if (count == 1)
		philo->forks[0] = &forks[0];
	else if (philo->id % 2 == 0)
	{
		philo->forks[0] = &forks[index];
		philo->forks[1] = &forks[(index + 1) % count];
	}
	else
	{
		philo->forks[0] = &forks[(index + 1) % count];
		philo->forks[1] = &forks[index];
	}
}

bool	philo_load(t_run *run, size_t index)
{
	t_philo	*philo;

	philo = &run->philos[index];
	if (mutex_load(run, &philo->mutex, true) == false)
		return (false);
	philo->id = index + 1;
	philo->meal_count = 0;
	philo_set_forks(philo, index, run->forks, run->args.philo_count);
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
