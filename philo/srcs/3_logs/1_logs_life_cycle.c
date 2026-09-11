#include "logs.h"
#include "mutex.h"
#include <stdio.h>

void	logs_init(t_logs *logs)
{
	logs->program_name = NULL;
	mutex_init(&logs->mutex);
	logs->closed = false;
}

bool	logs_load(t_run *run, const char *program_name)
{
	t_logs	*logs;

	logs = &run->logs;
	if (mutex_load(NULL, &logs->mutex, false) == false)
	{
		printf("%s: error: pthread_mutex_init() failed", logs->program_name);
		return (false);
	}
	logs->program_name = program_name;
	return (true);
}

void	logs_free(t_run *run)
{
	mutex_free(run, &run->logs.mutex, false);
	logs_init(&run->logs);
}
