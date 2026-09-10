#include "debug.h"
#include <stdio.h>

void	dump_philos(t_run *run)
{
	size_t	i;
	t_philo	*philo;

	i = 0;
	while (i < run->args.philo_count)
	{
		philo = &run->philos.list[i];
		printf("===> PHILO %zu | last_meal = %6lld | meal_count = %zu\n", philo->id, philo->last_meal, philo->meal_count);
		i++;
	}
}
