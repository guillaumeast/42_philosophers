#ifndef THREAD_H
# define THREAD_H

#include "types.h"

void	thread_init(t_thread *thread);
bool	thread_start(t_run *run, t_thread *thread, t_routine f, void *arg);
bool	thread_stop(t_run *run, t_thread *thread);
void	thread_free(t_run *run, t_thread *thread);

#endif
