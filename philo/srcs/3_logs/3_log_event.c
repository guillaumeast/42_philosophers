#include "logs.h"
#include "log_priv.h"
#include "run.h"
#include "mutex.h"
#include <stdio.h>

bool	log_fork(t_philo *philo, t_ms elapsed)
{
	return (log_priv(philo->run, elapsed, philo->id, "has taken a fork"));
}

bool	log_eat(t_philo *philo, t_ms elapsed)
{
	return (log_priv(philo->run, elapsed, philo->id, "is eating"));
}

bool	log_sleep(t_philo *philo, t_ms elapsed)
{
	return (log_priv(philo->run, elapsed, philo->id, "is sleeping"));
}

bool	log_think(t_philo *philo, t_ms elapsed)
{
	return (log_priv(philo->run, elapsed, philo->id, "is thinking"));
}

bool	log_death(t_philo *philo, t_ms elapsed)
{
	t_run	*run;
	bool	res;

	run = philo->run;
	if (run->logs.program_name == NULL)
	{
		(void)printf("philosophers: error: logs are not loaded yet");
		return (false);
	}
	res = run_stop(run, false, NULL);
	res = mutex_lock(run, &run->logs.mutex) && res == true;
	if (printf("%6lld %zu %s\n", elapsed, philo->id, "died") <= 0)
		res = run_stop(run, true, "printf() failed (a philosopher died)");
	return (mutex_unlock(run, &run->logs.mutex) == true && res == true);
}
