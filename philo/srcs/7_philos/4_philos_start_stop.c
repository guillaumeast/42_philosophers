#include "philos.h"
#include "philo_priv.h"
#include "thread.h"

bool	philos_start(t_run *run)
{
	size_t	i;
	t_philo	*philo;

	i = 0;
	while (i < run->args.philo_count)
	{
		philo = &run->philos[i];
		if (!thread_start(philo->run, &philo->thread, philo_routine, philo))
			return ((void)philos_stop(run, i), false);
		i++;
	}
	return (true);
}

bool	philos_stop(t_run *run, size_t count)
{
	size_t	i;
	t_philo	*philo;
	bool	success;

	success = true;
	i = 0;
	while (i < count)
	{
		philo = &run->philos[i];
		success = thread_stop(run, &philo->thread) && success == true;
		i++;
	}
	return (success);
}
