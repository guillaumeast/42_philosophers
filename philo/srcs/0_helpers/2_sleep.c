#include "helpers.h"
#include "clock.h"
#include "run.h"
#include <unistd.h>
#include <limits.h>

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
		if (clock_is_stopped(run, &stopped) == false)
			return (false);
		if (elapsed >= target_elapsed || stopped == true)
			return (true);
		if (sleep_cycle(run) == false)
			return (false);
	}
}

bool	sleep_for_ms(t_run *run, t_ms millisec)
{
	useconds_t	duration;

	if (millisec > UINT_MAX / 1000)
		return (run_stop(run, true, "sleep duration overflow"));
	duration = (useconds_t)(millisec * 1000);
	return (sleep_for_us(run, duration));
}

bool	sleep_for_us(t_run *run, useconds_t microsec)
{
	if (usleep(microsec) != 0)
		return (run_stop(run, true, "usleep() failed"));
	return (true);
}
