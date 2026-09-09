#ifndef MUTEX_H
#define MUTEX_H

# include "types.h"
# include <stdbool.h>

void	mutex_init(t_mutex *mutex);
bool	mutex_load(t_run *run, t_mutex *mutex, bool print_errors);
bool	mutex_lock(t_run *run, t_mutex *mutex);
bool	mutex_unlock(t_run *run, t_mutex *mutex);
void	mutex_free(t_run *run, t_mutex *mutex, bool print_errors);

#endif
