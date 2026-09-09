#ifndef ARGS_H
#define ARGS_H

# include "types.h"
# include <stdbool.h>

void	args_init(t_args *args);
bool	args_load(t_args *args, int argc, char **argv);
void	args_free(t_args *args);

#endif
