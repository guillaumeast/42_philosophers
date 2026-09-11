#ifndef LOGS_H
#define LOGS_H

# include "types.h"

# define GREY			"\033[0;90m"
# define BOLD_GREY		"\033[1;90m"
# define GREEN			"\033[0;32m"
# define BOLD_GREEN		"\033[1;32m"
# define BLUE			"\033[0;34m"
# define BOLD_BLUE		"\033[1;34m"
# define RED			"\033[0;31m"
# define BOLD_RED		"\033[1;31m"
# define YELLOW			"\033[0;33m"
# define BOLD_YELLOW	"\033[1;33m"
# define MAGENTA		"\033[0;35m"
# define BOLD_MAGENTA	"\033[1;35m"
# define CYAN			"\033[0;36m"
# define BOLD_CYAN		"\033[1;36m"
# define NC				"\033[0m"

// Life cycle

void	logs_init(t_logs *logs);
bool	logs_load(t_run *run, const char *program_name);
void	logs_free(t_run *run);

// Core

bool	log_fork(t_philo *philo, t_ms elapsed);
bool	log_eat(t_philo *philo);
bool	log_sleep(t_philo *philo, t_ms elapsed);
bool	log_think(t_philo *philo, t_ms elapsed);
bool	log_death(t_philo *philo, t_ms elapsed);
bool	log_stop(t_philo *philo, t_ms elapsed);
void	log_error(t_run *run, const char *opt_message);
void	log_philos(t_run *run);

#endif
