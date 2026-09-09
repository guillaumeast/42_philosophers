#ifndef FORKS_H
#define FORKS_H

# include "types.h"
# include <stdbool.h>
# include <stddef.h>

// Life cycle

void	forks_init(t_forks *forks);
bool	forks_load(t_run *run);
void	forks_free(t_run *run);

// Core

bool	forks_take(t_philo *philo, bool *out_taken);
bool	forks_drop(t_philo *philo);

#endif
