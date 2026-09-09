#ifndef ARGS_PRIV_H
#define ARGS_PRIV_H

# include "types.h"
# include <stdbool.h>
# include <stddef.h>

bool	args_parse_size_t(const char *arg, size_t *out, bool accept_zero);
bool	args_parse_ms(const char *arg, t_ms *out);

#endif
