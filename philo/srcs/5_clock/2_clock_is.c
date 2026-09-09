#include "clock.h"
#include "mutex.h"

bool	clock_is_started(t_run *run, bool *out)
{
	t_clock	*clock;

	clock = &run->clock;
	if (mutex_lock(run, &clock->mutex) == false)
		return (false);
	*out = clock->started == true || clock->stop == true;
	return (mutex_unlock(run, &clock->mutex));
}

bool	clock_is_stopped(t_run *run, bool *out)
{
	t_clock	*clock;

	clock = &run->clock;
	if (mutex_lock(run, &clock->mutex) == false)
		return (false);
	*out = clock->stop == true;
	return (mutex_unlock(run, &clock->mutex));
}

bool	clock_is_error(t_run *run)
{
	t_clock	*clock;
	bool	res;

	clock = &run->clock;
	if (mutex_lock(run, &clock->mutex) == false)
		return (false);
	res = clock->error;
	if (mutex_unlock(run, &clock->mutex) == false)
	{
		clock->error = true;
		return (true);
	}
	return (res);
}
