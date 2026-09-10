#include "log_priv.h"
#include "clock.h"
#include "mutex.h"
#include <stdio.h>

static inline void	log_select_format(t_log_type type, const char **out_suffix, const char **out_color)
{
	*out_color = NC;
	if (type == LOG_EAT)
	{
		*out_suffix = EAT_SUFFIX;
		*out_color = GREEN;
	}
	else if (type == LOG_THINK)
	{
		*out_suffix = THINK_SUFFIX;
		*out_color = YELLOW;
	}
	else if (type == LOG_FORK)
		*out_suffix = FORK_SUFFIX;
	else if (type == LOG_SLEEP)
		*out_suffix = SLEEP_SUFFIX;
}

static inline bool	log_formatted(t_run *run, t_ms elapsed, size_t philo_id, t_log_type type)
{
	const char	*suffix;
	const char	*color;

	log_select_format(type, &suffix, &color);
	if (printf("%s%6lld %zu %s\n" NC, color, elapsed, philo_id, suffix) <= 0)
		return ((void)clock_stop(run, true), false);
	return (true);
}

bool	log_priv(t_run *run, t_ms elapsed, size_t philo_id, t_log_type type)
{
	bool	res;
	bool	stopped;

	if (clock_is_stopped(run, &stopped) == false)
		return (false);
	if (stopped == true)
		return (true);
	if (mutex_lock(run, &run->logs.mutex) == false)
		return (false);
	res = true;
	if (type == LOG_EAT)
		res = log_formatted(run, elapsed, philo_id, LOG_FORK)
			&& log_formatted(run, elapsed, philo_id, LOG_FORK);
	if (res == true)
		res = log_formatted(run, elapsed, philo_id, type) && res == true;
	return (mutex_unlock(run, &run->logs.mutex) == true && res == true);
}
