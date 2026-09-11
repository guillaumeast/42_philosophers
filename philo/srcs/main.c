#include "run.h"
#include "logs.h"
#include <stdlib.h>

int	main(int argc, char **argv)
{
	t_run	run;

	run_init(&run);
	if (run_load(&run, argc, argv) == false)
		return (EXIT_FAILURE);
	if (run_start(&run) == false)
		return (run_free(&run), EXIT_FAILURE);
	log_philos(&run);
	run_free(&run);
	return (EXIT_SUCCESS);
}
