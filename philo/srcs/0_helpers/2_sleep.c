#include "helpers.h"
#include "clock.h"
#include "run.h"
#include <unistd.h>

static inline bool	sleep_cycle(t_run *run)
{
	if (usleep(SLEEP_DURATION_US) != 0)
		return (run_stop(run, true, "usleep() failed"));
	return (true);
}

bool	sleep_until(t_run *run, t_ms target_elapsed)
{
	t_ms	elapsed;
	bool	stopped;

	while (true)
	{
		if (clock_get_elapsed(run, &elapsed) == false)
			return (false);
		if (run_is_stopped(run, &stopped) == false)
			return (false);
		if (elapsed >= target_elapsed || stopped == true)
			return (true);
		if (sleep_cycle(run) == false)
			return (false);
	}
}

bool	sleep_for(t_run *run, t_ms duration)
{
	t_ms	now;
	t_ms	target;
	bool	stopped;

	if (time_now(run, &now) == false)
		return (false);
	if (time_try_add(run, now, duration, &target) == false)
		return (false);
	while (true)
	{
		if (time_now(run, &now) == false)
			return (false);
		if (run_is_stopped(run, &stopped) == false)
			return (false);
		if (now >= target || stopped == true)
			return (true);
		if (sleep_cycle(run) == false)
			return (false);
	}
}
