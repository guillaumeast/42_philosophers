#include "clock.h"
#include "mutex.h"
#include "logs.h"

void	clock_init(t_clock *clock)
{
	mutex_init(&clock->mutex);
	clock->start_ms = UNSET_MS;
	clock->started = false;
	clock->stop = false;
	clock->error = false;
}

bool	clock_load(t_run *run)
{
	if (mutex_load(run, &run->clock.mutex, true) == false)
		return (false);
	return (true);
}

bool	clock_start(t_run *run, t_ms start_ms)
{
	t_clock	*clock;

	clock = &run->clock;
	if (mutex_lock(run, &clock->mutex) == false)
		return (false);
	if (clock->started == true)
	{
		log_error(run, "clock has already started");
		clock->stop = true;
		clock->error = true;
	}
	clock->start_ms = start_ms;
	clock->started = true;
	if (mutex_unlock(run, &clock->mutex) == false)
	{
		clock->stop = true;
		clock->error = true;
		return (false);
	}
	return (true);
}

bool	clock_stop(t_run *run, bool error)
{
	if (run->clock.started == false)
		return (true);
	if (mutex_lock(run, &run->clock.mutex) == false)
		return (false);
	run->clock.stop = true;
	run->clock.error = run->clock.error || error;
	if (mutex_unlock(run, &run->clock.mutex) == false)
		return (run->clock.error = true, false);
	return (true);
}

void	clock_free(t_run *run)
{
	if (run->clock.start_ms == UNSET_MS)
		return ;
	mutex_free(run, &run->clock.mutex, true);
	clock_init(&run->clock);
}
