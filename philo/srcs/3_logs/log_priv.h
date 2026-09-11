#ifndef LOG_PRIV_H
# define LOG_PRIV_H

# include "types.h"

# define FORK_SUFFIX	"has taken a fork"
# define EAT_SUFFIX		"is eating"
# define SLEEP_SUFFIX	"is sleeping"
# define THINK_SUFFIX	"is thinking"
# define DEATH_SUFFIX	"died"
# define STOP_SUFFIX	"stopped"

typedef enum e_log_type
{
	LOG_FORK,
	LOG_EAT,
	LOG_SLEEP,
	LOG_THINK,
	LOG_DEATH,
	LOG_STOP
}	t_log_type;

const char	*log_suffix(t_log_type type);
bool		log_custom_formatted(t_philo *philo, t_ms elapsed, t_log_type type);
bool		log_priv(
				t_philo *philo,
				t_ms elapsed,
				bool state_locked,
				t_log_type type);

#endif
