#ifndef FORKS_H
#define FORKS_H

# include "types.h"
# include <stdbool.h>
# include <stddef.h>

void	forks_init(t_mutex **forks);
bool	forks_load(t_run *run);
void	forks_free(t_run *run, size_t count);

#endif
