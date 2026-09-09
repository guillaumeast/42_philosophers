#include "philos.h"
#include "philo_priv.h"
#include "run.h"
#include <stdlib.h>
#include <stdint.h>

void	philos_init(t_philo **philos)
{
	*philos = NULL;
}

bool	philos_load(t_run *run)
{
	size_t	i;

	if (run->args.philo_count > SIZE_MAX / sizeof(*run->philos))
		return (run_stop(run, true, "philos: malloc size overflow"));
	run->philos = malloc(run->args.philo_count * sizeof(*run->philos));
	if (run->philos == NULL)
		return (run_stop(run, true, "philos: malloc() failed"));
	i = 0;
	while (i < run->args.philo_count)
	{
		philo_init(&run->philos[i]);
		if (philo_load(run, i) == false)
			return (philos_free(run, i), false);
		i++;
	}
	return (true);
}

void	philos_free(t_run *run, size_t count)
{
	size_t	i;

	if (run->philos == NULL)
		return ;
	i = 0;
	while (i < count)
		philo_free(&run->philos[i++]);
	free(run->philos);
	philos_init(&run->philos);
}
