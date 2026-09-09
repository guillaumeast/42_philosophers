#include "forks.h"
#include "run.h"
#include "mutex.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

void	forks_init(t_mutex **forks)
{
	*forks = NULL;
}

bool	forks_load(t_run *run)
{
	t_mutex	**forks;
	size_t	count;
	size_t	i;

	forks = &run->forks;
	count = run->args.philo_count;
	if (count > SIZE_MAX / sizeof(**forks))
		return (run_stop(run, true, "forks: malloc size overflow"));
	*forks = malloc(count * sizeof(**forks));
	if (*forks == NULL)
		return (run_stop(run, true, "forks: malloc() failed"));
	i = 0;
	while (i < count)
	{
		if (mutex_load(run, &(*forks)[i], true) == false)
			return (forks_free(run, i), false);
		i++;
	}
	return (true);
}

void	forks_free(t_run *run, size_t count)
{
	size_t	i;

	if (run->forks == NULL)
		return ;
	i = 0;
	while (i < count)
	{
		mutex_free(run, &run->forks[i], true);
		i++;
	}
	free(run->forks);
	forks_init(&run->forks);
}
