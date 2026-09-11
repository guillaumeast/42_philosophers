#ifndef TYPES_H
#define TYPES_H

# include <pthread.h>
# include <stdbool.h>
# include <stddef.h>

# define SLEEP_DURATION_US	200

# define UNSET_MS			-1
# define UNSET_SIZE_T		0

typedef void			*(*t_routine)(void *);
typedef long long		t_ms;
typedef struct s_run	t_run;

typedef struct s_mutex
{
	bool			loaded;
	pthread_mutex_t	var;
}	t_mutex;

typedef struct s_thread
{
	bool		active;
	pthread_t	data;
}	t_thread;

typedef struct s_logs
{
	t_mutex		mutex;
	const char	*program_name;	// borrowed from argv[0]
	bool		closed;
}	t_logs;

typedef struct s_args
{
	size_t	philo_count;
	t_ms	time_to_die;
	t_ms	time_to_eat;
	t_ms	time_to_sleep;
	bool	meals_is_set;
	size_t	meals_count;
	bool	custom_logs;
}	t_args;

typedef struct s_clock
{
	t_ms		start_ms;
	bool		started;
	bool		stop;
	bool		error;
}	t_clock;

typedef struct s_philo
{
	size_t			id;
	t_thread		thread;
	t_ms			last_meal;
	size_t			meal_count;
	bool			*fork_left_is_available;	// borrowed
	bool			*fork_right_is_available;	// borrowed
	struct s_philo	*philo_left;				// borrowed
	struct s_philo	*philo_right;				// borrowed
	t_run			*run;						// borrowed
}	t_philo;

typedef struct s_philos
{
	t_philo	*list;				// owned
	size_t	count;
}	t_philos;

typedef struct s_forks
{
	bool	*available;			// owned
	size_t	count;
}	t_forks;

typedef struct s_run
{
	t_mutex		mutex;
	t_logs		logs;
	t_args		args;
	t_clock		clock;
	t_forks		forks;
	t_philos	philos;
}	t_run;

#endif
