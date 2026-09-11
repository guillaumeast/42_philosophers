#ifndef CLOCK_H
# define CLOCK_H

# include "types.h"

// Life cycle

void	clock_init(t_clock *clock);
bool	clock_load(t_run *run);
void	clock_free(t_run *run);

// Core

bool	clock_start(t_run *run, t_ms start_ms);
bool	clock_get_start_ms(t_run *run, t_ms *out);
bool	clock_stop(t_run *run, bool error);

// Getters

bool	clock_is_started(t_run *run, bool *out);
bool	clock_is_stopped(t_run *run, bool mutex_is_locked, bool *out);
bool	clock_is_error(t_run *run);
bool	clock_get_elapsed(t_run *run, t_ms *out);

#endif
