#include "mutex.h"
#include "run.h"

void	mutex_init(t_mutex *mutex)
{
	mutex->loaded = false;
}

bool	mutex_load(t_run *run, t_mutex *mutex, bool report_errors)
{
	if (mutex->loaded == true)
	{
		if (report_errors == true)
			return (run_panic(run, "mutex is already loaded"));
		return (false);
	}
	if (pthread_mutex_init(&mutex->var, NULL) != 0)
	{
		if (report_errors == true)
			return (run_panic(run, "pthread_mutex_init() failed"));
		return (false);
	}
	mutex->loaded = true;
	return (true);
}

bool	mutex_lock(t_run *run, t_mutex *mutex)
{
	if (mutex->loaded == false)
		return (run_panic(run, "mutex is not loaded yet"));
	if (pthread_mutex_lock(&mutex->var) != 0)
		return (run_panic(run, "pthread_mutex_lock() failed"));
	return (true);
}

bool	mutex_unlock(t_run *run, t_mutex *mutex)
{
	if (mutex->loaded == false)
		return (run_panic(run, "mutex is not loaded yet"));
	if (pthread_mutex_unlock(&mutex->var) != 0)
		return (run_panic(run, "pthread_mutex_unlock() failed"));
	return (true);
}

void	mutex_free(t_run *run, t_mutex *mutex, bool report_errors)
{
	if (mutex->loaded == false)
		return ;
	if (pthread_mutex_destroy(&mutex->var) != 0)
	{
		if (report_errors == true)
			(void)run_panic(run, "pthread_mutex_destroy() failed");
		return ;
	}
	mutex->loaded = false;
}
