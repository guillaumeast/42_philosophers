#include "philos.h"
#include "philo_priv.h"
#include "run.h"
#include <stdlib.h>
#include <stdint.h>

void	philos_init(t_philos *philos)
{
	philos->list = NULL;
	philos->count = UNSET_SIZE_T;
}

bool	philos_load(t_run *run)
{
	size_t	i;

	run->philos.count = run->args.philo_count;
	if (run->philos.count > SIZE_MAX / sizeof(*run->philos.list))
		return (run_stop(run, true, "philos: malloc size overflow"));
	run->philos.list = malloc(run->philos.count * sizeof(*run->philos.list));
	if (run->philos.list == NULL)
		return (run_stop(run, true, "philos: malloc() failed"));
	i = 0;
	while (i < run->philos.count)
	{
		philo_init(&run->philos.list[i]);
		if (philo_load(run, i) == false)
			return (philos_free(run, i), false);
		i++;
	}
	return (true);
}

void	philos_free(t_run *run, size_t count)
{
	size_t	i;

	if (run->philos.list != NULL)
	{
		i = 0;
		while (i < count)
			philo_free(&run->philos.list[i++]);
	}
	free(run->philos.list);
	philos_init(&run->philos);
}
