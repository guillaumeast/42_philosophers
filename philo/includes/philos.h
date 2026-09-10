#ifndef PHILOS_H
#define PHILOS_H

# include "types.h"
# include <stdbool.h>
# include <stddef.h>

// Life cycle

void	philos_init(t_philos *philos);
bool	philos_load(t_run *run);
void	philos_free(t_run *run, size_t count);

// Core

bool	philos_start(t_run *run);
bool	philos_stop(t_run *run, size_t count);

#endif
