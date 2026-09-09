#include "log_priv.h"
#include "clock.h"
#include "mutex.h"
#include <stdio.h>

static inline bool	log_formatted(t_run *run, t_ms elapsed, size_t philo_id, const char *suffix)
{
	if (printf("%6lld %zu %s\n", elapsed, philo_id, suffix) <= 0)
		return ((void)clock_stop(run, true), false);
	return (true);
}

bool	log_priv(t_run *run, t_ms elapsed, size_t philo_id, const char *suffix)
{
	bool	res;
	bool	stopped;

	if (clock_is_stopped(run, &stopped) == false)
		return (false);
	if (stopped == true)
		return (true);
	if (mutex_lock(run, &run->logs.mutex) == false)
		return (false);
	res = log_formatted(run, elapsed, philo_id, suffix);
	return (mutex_unlock(run, &run->logs.mutex) == true && res == true);
}
