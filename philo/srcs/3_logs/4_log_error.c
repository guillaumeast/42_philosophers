#include "logs.h"
#include "mutex.h"
#include <stdio.h>

void	log_error(t_run *run, const char *opt_message)
{
	if (run->logs.program_name == NULL)
	{
		if (opt_message == NULL)
			(void)printf("philosophers: error: logs are not loaded yet\n");
		else
			(void)printf("philosophers: error: logs are not loaded yet (initial log: %s)\n", opt_message);
		return ;
	}
	(void)mutex_lock(run, &run->logs.mutex);
	if (run->logs.error_printed == false)
	{
		if (opt_message == NULL)
			(void)printf("%s: error\n", run->logs.program_name);
		else
			(void)printf("%s: error: %s\n", run->logs.program_name, opt_message);
		run->logs.error_printed = true;
	}
	(void)mutex_unlock(run, &run->logs.mutex);
}
