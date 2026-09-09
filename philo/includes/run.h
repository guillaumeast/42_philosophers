#ifndef RUN_H
#define RUN_H

# include "types.h"
# include <stdbool.h>

void	run_init(t_run *run);
bool	run_load(t_run *run, int argc, char **argv);
void	run_free(t_run *run);

bool	run_start(t_run *run);
bool	run_stop(t_run *run, bool error, const char *opt_message);

#endif
