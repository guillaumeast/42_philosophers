#include "run.h"
#include "logs.h"

// Updates the stop state without locking because synchronization has failed.
bool	run_panic(t_run *run, const char *message)
{
	if (run != NULL)
	{
		run->logs.closed = true;
		run->logs.error = true;
		run->clock.stop = true;
		run->clock.error = true;
	}
	return (log_panic(run, message));
}
