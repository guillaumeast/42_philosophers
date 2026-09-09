#include "clock.h"
#include "helpers.h"
#include "run.h"
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

bool	clock_get_now(t_run *run, t_ms *out)
{
	return (time_now(run, out));
}

bool	clock_get_start_ms(t_run *run, t_ms *out)
{
	if (run->clock.start_ms == UNSET_MS)
		return (run_stop(run, true, "clock has not started yet"));
	*out = run->clock.start_ms;
	return (true);
}

bool	clock_get_elapsed(t_run *run, t_ms *out)
{
	t_ms	now;
	t_ms	start_ms;

	if (time_now(run, &now) == false)
		return (false);
	if (clock_get_start_ms(run, &start_ms) == false)
		return (false);
	*out = now - start_ms;
	return (true);
}
