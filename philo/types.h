#ifndef TYPES_H
#define TYPES_H

# include <pthread.h>
# include <stdbool.h>
# include <stddef.h>
# include <sys/types.h>

typedef long long		t_ms;
typedef struct s_run	t_run;

typedef struct s_args
{
	size_t	number_of_philosophers;
	t_ms	time_to_die;
	t_ms	time_to_eat;
	t_ms	time_to_sleep;
	ssize_t	number_of_times_each_philosopher_must_eat;
}	t_args;

typedef struct s_philo
{
	pthread_mutex_t	mutex;
	int				id;
	pthread_t		thread;
	t_ms			last_meal;
	size_t			meal_count;
	pthread_mutex_t	*left_fork;		// borrowed
	pthread_mutex_t	*right_fork;	// borrowed
	t_run			*run;			// borrowed
}	t_philo;

typedef struct s_run
{
	t_args			args;
	pthread_mutex_t	*forks;			// owned
	t_philo			*philosophers;	// owned
	t_ms			start_time;
	pthread_mutex_t	stop_mutex;
	bool			stop;
	pthread_mutex_t	print_mutex;
}	t_run;

#endif
