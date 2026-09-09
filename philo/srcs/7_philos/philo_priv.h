#ifndef PHILO_PRIV_H
#define PHILO_PRIV_H

# include "types.h"

void	philo_init(t_philo *philo);
bool	philo_load(t_run *run, size_t index);
void	philo_free(t_philo *philo);

bool	philo_eat(t_philo *philo);
bool	philo_sleep(t_philo *philo);
bool	philo_think(t_philo *philo);

void	*philo_routine(void *arg);

bool	philo_increment_meals(t_philo *philo);

#endif
