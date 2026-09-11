#include "helpers.h"
#include "run.h"
#include <limits.h>
#include <sys/time.h>

static bool	time_error(t_run *run, bool state_locked, const char *message)
{
	if (state_locked == true)
		return (run_stop_locked(run, true, message));
	return (run_stop(run, true, message));
}

bool	time_now(t_run *run, bool state_locked, t_ms *out)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) != 0)
		return (time_error(run, state_locked, "gettimeofday() failed"));
	if (tv.tv_sec > (LLONG_MAX - (t_ms)tv.tv_usec / 1000) / 1000)
		return (time_error(run, state_locked, "time now overflow"));
	*out = (t_ms)tv.tv_sec * 1000 + (t_ms)tv.tv_usec / 1000;
	return (true);
}

bool	time_try_add(t_run *run, t_ms a, t_ms b, t_ms *out)
{
	if (a > LLONG_MAX - b)
		return (run_stop(run, true, "time addition overflow"));
	*out = a + b;
	return (true);
}
