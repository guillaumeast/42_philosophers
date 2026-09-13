#ifndef HELPERS_H
#define HELPERS_H

# include "types.h"
# include <stdbool.h>
# include <sys/types.h>
# include <unistd.h>

// Time

bool	time_now(t_run *run, bool state_locked, t_ms *out);
bool	time_try_add(t_run *run, t_ms a, t_ms b, t_ms *out);

// Sleep

bool	sleep_until(t_run *run, t_ms target_elapsed);
bool	sleep_for_ms(t_run *run, t_ms millisec);
bool	sleep_for_us(t_run *run, useconds_t microsec);

#endif
