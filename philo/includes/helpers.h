#ifndef HELPERS_H
#define HELPERS_H

# include "types.h"
# include <stdbool.h>

// Time

bool	time_now(t_run *run, t_ms *out);
bool	time_try_add(t_run *run, t_ms a, t_ms b, t_ms *out);

// Sleep

bool	sleep_until(t_run *run, t_ms target_elapsed);
bool	sleep_for(t_run *run, t_ms duration);

#endif
