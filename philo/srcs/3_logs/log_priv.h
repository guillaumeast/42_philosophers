#ifndef LOG_PRIV_H
#define LOG_PRIV_H

# include "types.h"

typedef enum e_log_type
{
	LOG_FORK,
	LOG_EAT,
	LOG_SLEEP,
	LOG_THINK
}	t_log_type;

# define FORK_SUFFIX	"has taken a fork"
# define EAT_SUFFIX		"is eating"
# define SLEEP_SUFFIX	"is sleeping"
# define THINK_SUFFIX	"is thinking"

bool	log_priv(t_run *run, t_ms elapsed, size_t philo_id, t_log_type type);

#endif
