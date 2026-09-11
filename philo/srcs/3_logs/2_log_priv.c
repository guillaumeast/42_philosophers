#include "logs.h"
#include "log_priv.h"
#include "clock.h"
#include "mutex.h"
#include <stdio.h>

static inline void	log_select_format_(
						t_log_type type,
						const char **out_suffix,
						const char **out_color)
{
	if (type == LOG_FORK)
		*out_suffix = FORK_SUFFIX;
	if (type == LOG_EAT)
	{
		*out_suffix = EAT_SUFFIX;
		*out_color = GREEN;
	}
	else if (type == LOG_SLEEP)
		*out_suffix = SLEEP_SUFFIX;
	else if (type == LOG_THINK)
	{
		*out_suffix = THINK_SUFFIX;
		*out_color = YELLOW;
	}
	else if (type == LOG_DEATH)
	{
		*out_suffix = DEATH_SUFFIX;
		*out_color = RED;
	}
	else if (type == LOG_STOP)
	{
		*out_suffix = STOP_SUFFIX;
		*out_color = CYAN;
	}
}


static inline void	log_select_format(
						t_run *run,
						t_log_type type,
						const char **out_suffix,
						const char **out_color)
{
	*out_color = NC;
	log_select_format_(type, out_suffix, out_color);
	if (run->args.custom_logs == false)
		*out_color = NC;
}

static inline bool	log_formatted(
						t_run *run,
						t_ms elapsed,
						size_t philo_id,
						t_log_type type)
{
	const char	*suffix;
	const char	*color;

	log_select_format(run, type, &suffix, &color);
	if (printf("%s%6lld %3zu %s\n" NC, color, elapsed, philo_id, suffix) <= 0)
		return ((void)clock_stop(run, true), false);
	return (true);
}

bool	log_priv_safe(t_philo *philo, t_ms elapsed, t_log_type type)
{
	if (philo->run->logs.closed && type != LOG_STOP)
		return (true);
	philo->run->logs.closed = type == LOG_DEATH;
	if (type == LOG_EAT)
	{
		return (log_formatted(philo->run, elapsed, philo->id, LOG_FORK)
			&& log_formatted(philo->run, elapsed, philo->id, LOG_FORK)
			&& log_formatted(philo->run, elapsed, philo->id, type));
	}
	return (log_formatted(philo->run, elapsed, philo->id, type));
}

bool	log_priv(t_philo *philo, t_ms elapsed, bool locked, t_log_type type)
{
	t_run	*run;
	bool	res;

	run = philo->run;
	if (!locked && !mutex_lock(run, &run->logs.mutex))
		return (false);
	res = log_priv_safe(philo, elapsed, type);
	return ((locked || mutex_unlock(run, &run->logs.mutex)) && res);
}
