#include "log_priv.h"
#include "mutex.h"
#include "run.h"
#include <stdio.h>

const char	*log_suffix(t_log_type type)
{
	if (type == LOG_FORK)
		return (FORK_SUFFIX);
	if (type == LOG_EAT)
		return (EAT_SUFFIX);
	if (type == LOG_SLEEP)
		return (SLEEP_SUFFIX);
	if (type == LOG_THINK)
		return (THINK_SUFFIX);
	if (type == LOG_DEATH)
		return (DEATH_SUFFIX);
	if (type == LOG_STOP)
		return (STOP_SUFFIX);
	return ("unknown");
}

static inline bool	log_mandatory(
							t_philo *philo,
							t_ms elapsed,
							t_log_type type)
{
	return (printf("%lld %zu %s\n", elapsed, philo->id,
			log_suffix(type)) > 0);
}

static inline bool	log_formatted(
						t_philo *philo,
						t_ms elapsed,
						t_log_type type)
{
	t_run	*run;
	bool	success;

	run = philo->run;
	if (run->args.custom_logs == true)
		success = log_custom_formatted(philo, elapsed, type);
	else
		success = log_mandatory(philo, elapsed, type);
	if (success == false)
	{
		run->logs.closed = true;
		run->logs.error = true;
		return (false);
	}
	return (true);
}

bool	log_priv_safe(t_philo *philo, t_ms elapsed, t_log_type type)
{
	if (philo->run->logs.error
		|| (philo->run->logs.closed && type != LOG_STOP))
		return (true);
	if (type == LOG_DEATH)
		philo->run->logs.closed = true;
	if (type == LOG_EAT)
	{
		return (log_formatted(philo, elapsed, LOG_FORK)
			&& log_formatted(philo, elapsed, LOG_FORK)
			&& log_formatted(philo, elapsed, type));
	}
	return (log_formatted(philo, elapsed, type));
}

bool	log_priv(
			t_philo *philo,
			t_ms elapsed,
			bool state_locked,
			t_log_type type)
{
	t_run	*run;
	bool	success;

	run = philo->run;
	if (mutex_lock(run, &run->logs.mutex) == false)
		return (false);
	success = log_priv_safe(philo, elapsed, type);
	if (mutex_unlock(run, &run->logs.mutex) == false)
		return (false);
	if (success == true)
		return (true);
	if (state_locked == true)
		return (run_stop_locked(run, true, "printf() failed"));
	return (run_stop(run, true, "printf() failed"));
}
