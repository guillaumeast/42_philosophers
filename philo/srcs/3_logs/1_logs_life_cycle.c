#include "logs.h"
#include "mutex.h"

void	logs_init(t_logs *logs)
{
	logs->program_name = NULL;
	mutex_init(&logs->mutex);
	logs->closed = false;
	logs->error = false;
}

bool	logs_load(t_run *run, const char *program_name)
{
	t_logs	*logs;

	logs = &run->logs;
	logs->program_name = program_name;
	if (mutex_load(run, &logs->mutex, true) == false)
		return (false);
	return (true);
}

void	logs_free(t_run *run)
{
	mutex_free(run, &run->logs.mutex, true);
	logs_init(&run->logs);
}

bool	logs_close(t_run *run)
{
	if (mutex_lock(run, &run->logs.mutex) == false)
		return (false);
	run->logs.closed = true;
	return (mutex_unlock(run, &run->logs.mutex));
}
