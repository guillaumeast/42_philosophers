#ifndef TYPES_H
#define TYPES_H

# include <pthread.h>
# include <stdbool.h>
# include <stddef.h>

// tmp
#define GREY "\033[0;90m"
#define BOLD_GREY "\033[1;90m"
#define GREEN "\033[0;32m"
#define BOLD_GREEN "\033[1;32m"
#define BLUE "\033[0;34m"
#define BOLD_BLUE "\033[1;34m"
#define RED "\033[0;31m"
#define BOLD_RED "\033[1;31m"
#define YELLOW "\033[0;33m"
#define BOLD_YELLOW "\033[1;33m"
#define MAGENTA "\033[0;35m"
#define BOLD_MAGENTA "\033[1;35m"
#define CYAN "\033[0;36m"
#define BOLD_CYAN "\033[1;36m"
#define NC "\033[0m"

# define UNSET_MS			-1
# define UNSET_SIZE_T		0
# define SLEEP_DURATION_US	500

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
	bool		error_printed;
}	t_logs;

typedef struct s_args
{
	size_t	philo_count;
	t_ms	time_to_die;
	t_ms	time_to_eat;
	t_ms	time_to_sleep;
	bool	meals_is_set;
	size_t	meals_count;
}	t_args;

typedef struct s_clock
{
	t_mutex		mutex;
	t_ms		start_ms;
	bool		started;
	bool		stop;
	bool		error;
}	t_clock;

typedef struct s_philo
{
	t_mutex		mutex;
	size_t		id;
	t_thread	thread;
	t_ms		last_meal;
	size_t		meal_count;
	size_t		left_fork;
	size_t		right_fork;
	t_run		*run;			// borrowed
}	t_philo;

typedef struct s_forks
{
	t_mutex	mutex;
	size_t	round;
	size_t	meals_eaten;
	bool	*available;			// owned
	size_t	count;
}	t_forks;

typedef struct s_run
{
	t_logs			logs;
	t_args			args;
	t_clock			clock;
	t_forks			forks;
	t_philo			*philos;	// owned
}	t_run;

#endif
