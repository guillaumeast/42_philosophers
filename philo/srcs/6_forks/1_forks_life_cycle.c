#include "forks.h"
#include "run.h"
#include "mutex.h"
#include <stdlib.h>
#include <stdint.h>

void	forks_init(t_forks *forks)
{
	mutex_init(&forks->mutex);
	forks->round = UNSET_SIZE_T;
	forks->meals_eaten = UNSET_SIZE_T;
	forks->available = NULL;
	forks->count = UNSET_SIZE_T;
}

bool	forks_load(t_run *run)
{
	t_forks	*forks;
	size_t	i;

	forks = &run->forks;
	forks->count = run->args.philo_count;
	if (forks->count > SIZE_MAX / sizeof(*forks->available))
		return (run_stop(run, true, "forks: malloc size overflow"));
	forks->available = malloc(forks->count * sizeof(*forks->available));
	if (forks->available == NULL)
		return (run_stop(run, true, "forks: malloc() failed"));
	if (mutex_load(run, &run->forks.mutex, true) == false)
		return (free(forks->available), false);
	i = 0;
	while (i < forks->count)
		forks->available[i++] = true;
	return (true);
}

void	forks_free(t_run *run)
{
	mutex_free(run, &run->forks.mutex, true);
	if (run->forks.available != NULL)
		free(run->forks.available);
	forks_init(&run->forks);
}
