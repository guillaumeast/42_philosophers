#include "clock.h"
#include "mutex.h"
#include "run.h"

void	clock_init(t_clock *clock)
{
	clock->start_ms = UNSET_MS;
	clock->started = false;
	clock->stop = false;
	clock->error = false;
}

bool	clock_load(t_run *run)
{
	(void)run;
	return (true);
}

bool	clock_start(t_run *run, t_ms start_ms)
{
	t_clock	*clock;

	clock = &run->clock;
	if (mutex_lock(run, &run->mutex) == false)
		return (false);
	if (clock->started == true)
	{
		(void)run_stop_locked(run, true, "clock has already started");
		(void)mutex_unlock(run, &run->mutex);
		return (false);
	}
	clock->start_ms = start_ms;
	clock->started = true;
	if (mutex_unlock(run, &run->mutex) == false)
	{
		clock->stop = true;
		clock->error = true;
		return (false);
	}
	return (true);
}

void	clock_free(t_run *run)
{
	clock_init(&run->clock);
}
