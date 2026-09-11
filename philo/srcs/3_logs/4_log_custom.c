#include "logs.h"
#include "log_priv.h"
#include "mutex.h"
#include <stdio.h>

bool	log_stop(t_philo *philo, t_ms elapsed)
{
	if (philo->run->args.custom_logs == false)
		return (true);
	return (log_priv(philo, elapsed, false, LOG_STOP));
}

void	log_error(t_run *run, const char *opt_message)
{
	if (run->args.custom_logs == false)
		return ;
	if (run->logs.program_name == NULL)
	{
		if (opt_message == NULL)
			(void)printf("philosophers: error: logs are not loaded yet\n");
		else
			(void)printf("philosophers: error: logs are not loaded yet "
				"(initial log: %s)\n", opt_message);
		return ;
	}
	(void)mutex_lock(run, &run->logs.mutex);
	if (opt_message == NULL)
		(void)printf("%s: error\n", run->logs.program_name);
	else
		(void)printf("%s: error: %s\n", run->logs.program_name, opt_message);
	(void)mutex_unlock(run, &run->logs.mutex);
}

void	log_philos(t_run *run)
{
	size_t	i;
	t_philo	*philo;

	if (run->args.custom_logs == false)
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
