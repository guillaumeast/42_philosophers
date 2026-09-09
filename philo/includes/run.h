#ifndef RUN_H
#define RUN_H

# include "types.h"
# include <stdbool.h>

void	run_init(t_run *run);
bool	run_load(t_run *run, int argc, char **argv);
void	run_free(t_run *run);

bool	run_start(t_run *run);
bool	run_stop(t_run *run, bool error, const char *opt_message);
bool	run_monitor(t_run *run);

bool	run_is_started(t_run *run, bool *out);
bool	run_get_start_ms(t_run *run, t_ms *out);
bool	run_is_stopped(t_run *run, bool *out);

#endif
