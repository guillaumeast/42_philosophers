#include "logs.h"
#include "log_priv.h"
#include "mutex.h"
#include <stdio.h>

static inline const char	*log_color(t_log_type type)
{
	if (type == LOG_EAT)
		return (GREEN);
	if (type == LOG_THINK)
		return (YELLOW);
	if (type == LOG_DEATH)
		return (RED);
	if (type == LOG_STOP)
		return (CYAN);
	return (NC);
}

bool	log_custom_formatted(t_philo *philo, t_ms elapsed, t_log_type type)
{
	return (printf("%s%6lld %3zu %s\n" NC, log_color(type), elapsed,
			philo->id, log_suffix(type)) > 0);
}

bool	log_stop(t_philo *philo, t_ms elapsed)
{
	if (philo->run->args.custom_logs == false)
		return (true);
	return (log_priv(philo, elapsed, false, LOG_STOP));
}

bool	log_error(t_run *run, const char *message)
{
	if (mutex_lock(run, &run->logs.mutex) == false)
		return (false);
	if (run->logs.error == false)
	{
		run->logs.closed = true;
		run->logs.error = true;
		(void)log_panic(run, message);
	}
	return ((void)mutex_unlock(run, &run->logs.mutex), false);
}

void	log_philos(t_run *run)
{
	size_t	i;
	t_philo	*philo;

	if (run->args.custom_logs == false || run->logs.error == true)
		return ;
	i = 0;
	while (i < run->args.philo_count)
	{
		philo = &run->philos.list[i];
		(void)printf(MAGENTA
			"       %3zu => %zu meals (last at %6lld ms)\n" NC,
			philo->id, philo->meal_count, philo->last_meal);
		i++;
	}
}
