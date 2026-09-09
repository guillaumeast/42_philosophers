#include "thread.h"
#include "run.h"

void	thread_init(t_thread *thread)
{
	thread->active = false;
}

bool	thread_start(t_run *run, t_thread *thread, t_routine routine, void *arg)
{
	if (thread->active == true)
		return (false);
	if (pthread_create(&thread->data, NULL, routine, arg) != 0)
		return (run_stop(run, true, "pthread_create() failed"));
	thread->active = true;
	return (true);
}

bool	thread_stop(t_run *run, t_thread *thread)
{
	if (thread->active == false)
		return (true);
	if (pthread_join(thread->data, NULL) != 0)
		return (run_stop(run, true, "pthread_join() failed"));
	thread_init(thread);
	return (true);
}

void	thread_free(t_run *run, t_thread *thread)
{
	(void)thread_stop(run, thread);
}
