#include "clock.h"
#include "helpers.h"
#include "run.h"

bool	clock_get_start_ms(t_run *run, bool state_locked, t_ms *out)
{
	if (run->clock.start_ms == UNSET_MS)
	{
		if (state_locked == true)
			return (run_stop_locked(run, true, "clock has not started yet"));
		return (run_stop(run, true, "clock has not started yet"));
	}
	*out = run->clock.start_ms;
	return (true);
}

bool	clock_get_elapsed(t_run *run, bool state_locked, t_ms *out)
{
	t_ms	now;
	t_ms	start_ms;

	if (time_now(run, state_locked, &now) == false)
		return (false);
	if (clock_get_start_ms(run, state_locked, &start_ms) == false)
		return (false);
	*out = now - start_ms;
	return (true);
}
