#include "helpers.h"
#include "run.h"
#include <limits.h>
#include <sys/time.h>

bool	time_now(t_run *run, t_ms *out)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) != 0)
		return (run_stop(run, true, "gettimeofday() failed"));
	if (tv.tv_sec > (LLONG_MAX - (t_ms)tv.tv_usec / 1000) / 1000)
		return (run_stop(run, true, "time now overflow"));
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
