#ifndef LOGS_H
#define LOGS_H

# include "types.h"

// Life cycle

void	logs_init(t_logs *logs);
bool	logs_load(t_run *run, const char *program_name);
void	logs_free(t_run *run);

// Events

bool	log_fork(t_philo *philo, t_ms elapsed);
bool	log_eat(t_philo *philo, t_ms elapsed);
bool	log_sleep(t_philo *philo, t_ms elapsed);
bool	log_think(t_philo *philo, t_ms elapsed);
bool	log_death(t_philo *philo, t_ms elapsed);

// Errors

void	log_error(t_run *run, const char *opt_message);

#endif
