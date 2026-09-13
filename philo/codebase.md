tree
```bash
philo
├── includes
│   ├── args.h
│   ├── clock.h
│   ├── forks.h
│   ├── helpers.h
│   ├── logs.h
│   ├── mutex.h
│   ├── philos.h
│   ├── run.h
│   ├── thread.h
│   └── types.h
├── Makefile
├── manual_test.log
├── philo
├── srcs
│   ├── 0_helpers
│   │   ├── 1_time.c
│   │   └── 2_sleep.c
│   ├── 1_mutex
│   │   └── 1_mutex_life_cycle.c
│   ├── 2_thread
│   │   └── 1_thread_life_cycle.c
│   ├── 3_logs
│   │   ├── 1_logs_life_cycle.c
│   │   ├── 2_log_priv.c
│   │   ├── 3_log_api.c
│   │   ├── 4_log_custom.c
│   │   ├── 5_log_panic.c
│   │   └── log_priv.h
│   ├── 4_args
│   │   ├── 0_args_parse.c
│   │   ├── 1_args_life_cycle.c
│   │   ├── 2_args_load.c
│   │   └── args_priv.h
│   ├── 5_clock
│   │   ├── 1_clock_life_cycle.c
│   │   ├── 2_clock_is.c
│   │   └── 3_clock_get.c
│   ├── 6_forks
│   │   ├── 1_forks_life_cycle.c
│   │   └── 2_forks_take_drop.c
│   ├── 7_philos
│   │   ├── 1_philo_life_cycle.c
│   │   ├── 2_philos_life_cycle.c
│   │   ├── 3_philo_loops.c
│   │   ├── 3_philo_routine.c
│   │   ├── 4_philos_start_stop.c
│   │   └── philo_priv.h
│   ├── 8_run
│   │   ├── 1_run_life_cycle.c
│   │   ├── 2_run_start_stop.c
│   │   ├── 3_run_monitor.c
│   │   ├── 4_run_panic.c
│   │   └── run_priv.h
│   └── main.c
└── tests
    └── test.zsh

13 directories, 45 files
```

./includes/args.h
```c
#ifndef ARGS_H
#define ARGS_H

# include "types.h"
# include <stdbool.h>

void	args_init(t_args *args);
bool	args_load(t_args *args, int argc, char **argv);
void	args_free(t_args *args);

#endif

```

./includes/clock.h
```c
#ifndef CLOCK_H
# define CLOCK_H

# include "types.h"

// Life cycle

void	clock_init(t_clock *clock);
bool	clock_load(t_run *run);
void	clock_free(t_run *run);

// Core

bool	clock_start(t_run *run, t_ms start_ms);
bool	clock_get_start_ms(t_run *run, bool state_locked, t_ms *out);

// Getters

bool	clock_is_started(t_run *run, bool *out);
bool	clock_is_stopped(t_run *run, bool mutex_is_locked, bool *out);
bool	clock_is_error(t_run *run);
bool	clock_get_elapsed(t_run *run, bool state_locked, t_ms *out);

#endif

```

./includes/forks.h
```c
#ifndef FORKS_H
#define FORKS_H

# include "types.h"
# include <stdbool.h>
# include <stddef.h>

// Life cycle

void	forks_init(t_forks *forks);
bool	forks_load(t_run *run);
void	forks_free(t_run *run);

// Core

bool	forks_take(t_philo *philo);
bool	forks_drop(t_philo *philo);

#endif

```

./includes/helpers.h
```c
#ifndef HELPERS_H
#define HELPERS_H

# include "types.h"
# include <stdbool.h>
# include <sys/types.h>
# include <unistd.h>

// Time

bool	time_now(t_run *run, bool state_locked, t_ms *out);
bool	time_try_add(t_run *run, t_ms a, t_ms b, t_ms *out);

// Sleep

bool	sleep_until(t_run *run, t_ms target_elapsed);
bool	sleep_for_ms(t_run *run, t_ms millisec);
bool	sleep_for_us(t_run *run, useconds_t microsec);

#endif

```

./includes/logs.h
```c
#ifndef LOGS_H
# define LOGS_H

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
bool	logs_close(t_run *run);

// Core

bool	log_fork(t_philo *philo, t_ms elapsed);
bool	log_eat(t_philo *philo);
bool	log_sleep(t_philo *philo, t_ms elapsed);
bool	log_think(t_philo *philo, t_ms elapsed);
bool	log_death_locked(t_philo *philo, t_ms elapsed);
bool	log_stop(t_philo *philo, t_ms elapsed);
bool	log_error(t_run *run, const char *message);
bool	log_panic(t_run *run, const char *message);
void	log_philos(t_run *run);

#endif

```

./includes/mutex.h
```c
#ifndef MUTEX_H
# define MUTEX_H

# include "types.h"
# include <stdbool.h>

void	mutex_init(t_mutex *mutex);
bool	mutex_load(t_run *run, t_mutex *mutex, bool report_errors);
bool	mutex_lock(t_run *run, t_mutex *mutex);
bool	mutex_unlock(t_run *run, t_mutex *mutex);
void	mutex_free(t_run *run, t_mutex *mutex, bool report_errors);

#endif

```

./includes/philos.h
```c
#ifndef PHILOS_H
#define PHILOS_H

# include "types.h"
# include <stdbool.h>
# include <stddef.h>

// Life cycle

void	philos_init(t_philos *philos);
bool	philos_load(t_run *run);
void	philos_free(t_run *run, size_t count);

// Core

bool	philos_start(t_run *run);
bool	philos_stop(t_run *run, size_t count);

#endif

```

./includes/run.h
```c
#ifndef RUN_H
# define RUN_H

# include "types.h"
# include <stdbool.h>

void	run_init(t_run *run);
bool	run_load(t_run *run, int argc, char **argv);
void	run_free(t_run *run);

bool	run_start(t_run *run);
bool	run_stop(t_run *run, bool error, const char *message);
bool	run_stop_locked(t_run *run, bool error, const char *message);
bool	run_panic(t_run *run, const char *message);

#endif

```

./includes/thread.h
```c
#ifndef THREAD_H
# define THREAD_H

#include "types.h"

void	thread_init(t_thread *thread);
bool	thread_start(t_run *run, t_thread *thread, t_routine f, void *arg);
bool	thread_stop(t_run *run, t_thread *thread);
void	thread_free(t_run *run, t_thread *thread);

#endif

```

./includes/types.h
```c
#ifndef TYPES_H
# define TYPES_H

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
	bool		error;
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

```

./Makefile
```makefile
NAME		:= philo
CC			:= cc
CFLAGS		:= -Wall -Wextra -Werror -g3 # -fsanitize=address
DEPFLAGS	:= -MMD -MP

INCLUDES	:= -Iincludes
SRCS		:= $(wildcard srcs/*.c) $(wildcard srcs/*/*.c)

OBJ_DIR		:= obj
OBJS		:= $(SRCS:%.c=$(OBJ_DIR)/%.o)
DEPS		:= $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o : %.c
	@mkdir -p $(dir $@)
	$(CC) $(DEPFLAGS) $(CFLAGS) $(INCLUDES) -c $< -o $@

test: re
	@./tests/test.zsh

clean:
	rm -rf $(OBJ_DIR) tests/logs

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(wildcard $(DEPS))

.PHONY: all test clean fclean re

```

./srcs/0_helpers/1_time.c
```c
#include "helpers.h"
#include "run.h"
#include <limits.h>
#include <sys/time.h>

static bool	time_error(t_run *run, bool state_locked, const char *message)
{
	if (state_locked == true)
		return (run_stop_locked(run, true, message));
	return (run_stop(run, true, message));
}

bool	time_now(t_run *run, bool state_locked, t_ms *out)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) != 0)
		return (time_error(run, state_locked, "gettimeofday() failed"));
	if (tv.tv_sec > (LLONG_MAX - (t_ms)tv.tv_usec / 1000) / 1000)
		return (time_error(run, state_locked, "time now overflow"));
	*out = (t_ms)tv.tv_sec * 1000 + (t_ms)tv.tv_usec / 1000;
	return (true);
}

bool	time_try_add(t_run *run, t_ms a, t_ms b, t_ms *out)
{
	if (a > LLONG_MAX - b)
		return (run_stop(run, true, "time addition overflow"));
	*out = a + b;
	return (true);
}

```

./srcs/0_helpers/2_sleep.c
```c
#include "helpers.h"
#include "clock.h"
#include "run.h"
#include <unistd.h>
#include <limits.h>

static inline bool	sleep_cycle(t_run *run)
{
	if (usleep(SLEEP_DURATION_US) != 0)
		return (run_stop(run, true, "usleep() failed"));
	return (true);
}

bool	sleep_until(t_run *run, t_ms target_elapsed)
{
	t_ms	elapsed;
	bool	stopped;

	while (true)
	{
		if (clock_is_stopped(run, false, &stopped) == false || stopped == true)
			return (false);
		if (clock_get_elapsed(run, false, &elapsed) == false)
			return (false);
		if (elapsed >= target_elapsed - 1)
			break ;
		if (sleep_cycle(run) == false)
			return (false);
	}
	while (elapsed < target_elapsed)
	{
		if (clock_get_elapsed(run, false, &elapsed) == false)
			return (false);
	}
	return (true);
}

bool	sleep_for_ms(t_run *run, t_ms millisec)
{
	useconds_t	duration;

	if (millisec > UINT_MAX / 1000)
		return (run_stop(run, true, "sleep duration overflow"));
	duration = (useconds_t)(millisec * 1000);
	return (sleep_for_us(run, duration));
}

bool	sleep_for_us(t_run *run, useconds_t microsec)
{
	if (usleep(microsec) != 0)
		return (run_stop(run, true, "usleep() failed"));
	return (true);
}

```

./srcs/1_mutex/1_mutex_life_cycle.c
```c
#include "mutex.h"
#include "run.h"

void	mutex_init(t_mutex *mutex)
{
	mutex->loaded = false;
}

bool	mutex_load(t_run *run, t_mutex *mutex, bool report_errors)
{
	if (mutex->loaded == true)
	{
		if (report_errors == true)
			return (run_panic(run, "mutex is already loaded"));
		return (false);
	}
	if (pthread_mutex_init(&mutex->var, NULL) != 0)
	{
		if (report_errors == true)
			return (run_panic(run, "pthread_mutex_init() failed"));
		return (false);
	}
	mutex->loaded = true;
	return (true);
}

bool	mutex_lock(t_run *run, t_mutex *mutex)
{
	if (mutex->loaded == false)
		return (run_panic(run, "mutex is not loaded yet"));
	if (pthread_mutex_lock(&mutex->var) != 0)
		return (run_panic(run, "pthread_mutex_lock() failed"));
	return (true);
}

bool	mutex_unlock(t_run *run, t_mutex *mutex)
{
	if (mutex->loaded == false)
		return (run_panic(run, "mutex is not loaded yet"));
	if (pthread_mutex_unlock(&mutex->var) != 0)
		return (run_panic(run, "pthread_mutex_unlock() failed"));
	return (true);
}

void	mutex_free(t_run *run, t_mutex *mutex, bool report_errors)
{
	if (mutex->loaded == false)
		return ;
	if (pthread_mutex_destroy(&mutex->var) != 0)
	{
		if (report_errors == true)
			(void)run_panic(run, "pthread_mutex_destroy() failed");
		return ;
	}
	mutex->loaded = false;
}

```

./srcs/2_thread/1_thread_life_cycle.c
```c
#include "thread.h"
#include "run.h"

void	thread_init(t_thread *thread)
{
	thread->active = false;
}

bool	thread_start(t_run *run, t_thread *thread, t_routine routine, void *arg)
{
	if (thread->active == true)
		return (false);
	if (pthread_create(&thread->data, NULL, routine, arg) != 0)
		return (run_stop(run, true, "pthread_create() failed"));
	thread->active = true;
	return (true);
}

bool	thread_stop(t_run *run, t_thread *thread)
{
	if (thread->active == false)
		return (true);
	if (pthread_join(thread->data, NULL) != 0)
		return (run_stop(run, true, "pthread_join() failed"));
	thread_init(thread);
	return (true);
}

void	thread_free(t_run *run, t_thread *thread)
{
	(void)thread_stop(run, thread);
}

```

./srcs/3_logs/1_logs_life_cycle.c
```c
#include "logs.h"
#include "mutex.h"

void	logs_init(t_logs *logs)
{
	logs->program_name = NULL;
	mutex_init(&logs->mutex);
	logs->closed = false;
	logs->error = false;
}

bool	logs_load(t_run *run, const char *program_name)
{
	t_logs	*logs;

	logs = &run->logs;
	logs->program_name = program_name;
	if (mutex_load(run, &logs->mutex, true) == false)
		return (false);
	return (true);
}

void	logs_free(t_run *run)
{
	mutex_free(run, &run->logs.mutex, true);
	logs_init(&run->logs);
}

bool	logs_close(t_run *run)
{
	if (mutex_lock(run, &run->logs.mutex) == false)
		return (false);
	run->logs.closed = true;
	return (mutex_unlock(run, &run->logs.mutex));
}

```

./srcs/3_logs/2_log_priv.c
```c
#include "log_priv.h"
#include "mutex.h"
#include "run.h"
#include <stdio.h>

const char	*log_suffix(t_log_type type)
{
	if (type == LOG_FORK)
		return (FORK_SUFFIX);
	if (type == LOG_EAT)
		return (EAT_SUFFIX);
	if (type == LOG_SLEEP)
		return (SLEEP_SUFFIX);
	if (type == LOG_THINK)
		return (THINK_SUFFIX);
	if (type == LOG_DEATH)
		return (DEATH_SUFFIX);
	if (type == LOG_STOP)
		return (STOP_SUFFIX);
	return ("unknown");
}

static inline bool	log_mandatory(
							t_philo *philo,
							t_ms elapsed,
							t_log_type type)
{
	return (printf("%lld %zu %s\n", elapsed, philo->id,
			log_suffix(type)) > 0);
}

static inline bool	log_formatted(
						t_philo *philo,
						t_ms elapsed,
						t_log_type type)
{
	t_run	*run;
	bool	success;

	run = philo->run;
	if (run->args.custom_logs == true)
		success = log_custom_formatted(philo, elapsed, type);
	else
		success = log_mandatory(philo, elapsed, type);
	if (success == false)
	{
		run->logs.closed = true;
		run->logs.error = true;
		return (false);
	}
	return (true);
}

bool	log_priv_safe(t_philo *philo, t_ms elapsed, t_log_type type)
{
	if (philo->run->logs.error
		|| (philo->run->logs.closed && type != LOG_STOP))
		return (true);
	if (type == LOG_DEATH)
		philo->run->logs.closed = true;
	if (type == LOG_EAT)
	{
		return (log_formatted(philo, elapsed, LOG_FORK)
			&& log_formatted(philo, elapsed, LOG_FORK)
			&& log_formatted(philo, elapsed, type));
	}
	return (log_formatted(philo, elapsed, type));
}

bool	log_priv(
			t_philo *philo,
			t_ms elapsed,
			bool state_locked,
			t_log_type type)
{
	t_run	*run;
	bool	success;

	run = philo->run;
	if (mutex_lock(run, &run->logs.mutex) == false)
		return (false);
	success = log_priv_safe(philo, elapsed, type);
	if (mutex_unlock(run, &run->logs.mutex) == false)
		return (false);
	if (success == true)
		return (true);
	if (state_locked == true)
		return (run_stop_locked(run, true, "printf() failed"));
	return (run_stop(run, true, "printf() failed"));
}

```

./srcs/3_logs/3_log_api.c
```c
#include "logs.h"
#include "log_priv.h"

bool	log_fork(t_philo *philo, t_ms elapsed)
{
	return (log_priv(philo, elapsed, false, LOG_FORK));
}

bool	log_eat(t_philo *philo)
{
	return (log_priv(philo, philo->last_meal, false, LOG_EAT));
}

bool	log_sleep(t_philo *philo, t_ms elapsed)
{
	return (log_priv(philo, elapsed, false, LOG_SLEEP));
}

bool	log_think(t_philo *philo, t_ms elapsed)
{
	return (log_priv(philo, elapsed, false, LOG_THINK));
}

bool	log_death_locked(t_philo *philo, t_ms elapsed)
{
	return (log_priv(philo, elapsed, true, LOG_DEATH));
}

```

./srcs/3_logs/4_log_custom.c
```c
#include "logs.h"
#include "log_priv.h"
#include "mutex.h"
#include <stdio.h>

static inline const char	*log_color(t_log_type type)
{
	if (type == LOG_EAT)
		return (GREEN);
	if (type == LOG_THINK)
		return (YELLOW);
	if (type == LOG_DEATH)
		return (RED);
	if (type == LOG_STOP)
		return (CYAN);
	return (NC);
}

bool	log_custom_formatted(t_philo *philo, t_ms elapsed, t_log_type type)
{
	return (printf("%s%6lld %3zu %s\n" NC, log_color(type), elapsed,
			philo->id, log_suffix(type)) > 0);
}

bool	log_stop(t_philo *philo, t_ms elapsed)
{
	if (philo->run->args.custom_logs == false)
		return (true);
	return (log_priv(philo, elapsed, false, LOG_STOP));
}

bool	log_error(t_run *run, const char *message)
{
	if (mutex_lock(run, &run->logs.mutex) == false)
		return (false);
	if (run->logs.error == false)
	{
		run->logs.closed = true;
		run->logs.error = true;
		(void)log_panic(run, message);
	}
	return ((void)mutex_unlock(run, &run->logs.mutex), false);
}

void	log_philos(t_run *run)
{
	size_t	i;
	t_philo	*philo;

	if (run->args.custom_logs == false || run->logs.error == true)
		return ;
	i = 0;
	while (i < run->args.philo_count)
	{
		philo = &run->philos.list[i];
		(void)printf(MAGENTA
			"       %3zu => %zu meals (last at %6lld ms)\n" NC,
			philo->id, philo->meal_count, philo->last_meal);
		i++;
	}
}

```

./srcs/3_logs/5_log_panic.c
```c
#include "logs.h"
#include <unistd.h>

static size_t	str_len(const char *string)
{
	size_t	len;

	len = 0;
	while (string[len] != '\0')
		len++;
	return (len);
}

bool	log_panic(t_run *run, const char *message)
{
	const char	*program_name;

	program_name = "philosophers";
	if (run != NULL && run->logs.program_name != NULL)
		program_name = run->logs.program_name;
	(void)write(STDERR_FILENO, program_name, str_len(program_name));
	if (message == NULL)
		(void)write(STDERR_FILENO, ": error", 7);
	else
	{
		(void)write(STDERR_FILENO, ": error: ", 9);
		(void)write(STDERR_FILENO, message, str_len(message));
	}
	(void)write(STDERR_FILENO, "\n", 1);
	return (false);
}

```

./srcs/3_logs/log_priv.h
```c
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

```

./srcs/4_args/0_args_parse.c
```c
#include "args_priv.h"
#include <limits.h>
#include <stdint.h>

bool	args_parse_size_t(const char *arg, size_t *out, bool accept_zero)
{
	size_t	result;
	int		digit;

	result = 0;
	if (*arg == '+')
		arg++;
	if (*arg == '\0')
		return (false);
	while (*arg)
	{
		if (*arg < '0' || *arg > '9')
			return (false);
		digit = *arg - '0';
		if (result > (SIZE_MAX - (size_t)digit) / 10)
			return (false);
		result = result * 10 + (size_t)digit;
		arg++;
	}
	if (result == 0 && accept_zero == false)
		return (false);
	*out = result;
	return (true);
}

bool	args_parse_ms(const char *arg, t_ms *out)
{
	t_ms	result;
	int		digit;

	result = 0;
	if (*arg == '+')
		arg++;
	if (*arg == '\0')
		return (false);
	while (*arg)
	{
		if (*arg < '0' || *arg > '9')
			return (false);
		digit = *arg - '0';
		if (result > (LLONG_MAX - (t_ms)digit) / 10)
			return (false);
		result = result * 10 + (t_ms)digit;
		arg++;
	}
	*out = result;
	return (true);
}

```

./srcs/4_args/1_args_life_cycle.c
```c
#include "args.h"

void	args_init(t_args *args)
{
	args->philo_count = UNSET_SIZE_T;
	args->time_to_die = UNSET_MS;
	args->time_to_eat = UNSET_MS;
	args->time_to_sleep = UNSET_MS;
	args->meals_is_set = false;
	args->meals_count = UNSET_SIZE_T;
	args->custom_logs = false;
}

void	args_free(t_args *args)
{
	args_init(args);
}

```

./srcs/4_args/2_args_load.c
```c
#include "args.h"
#include "args_priv.h"
#include <limits.h>
#include <stdio.h>

#define OPERAND_MIN		4
#define OPERAND_MAX		5
#define OPT_ARG_NAME	"number of times each philosopher must eat"

static inline bool	args_err(
						t_args *args,
						const char *program_name,
						const char *arg_value,
						const char *arg_name)
{
	(void)printf("%s: error: '%s': invalid %s.\n",
		program_name, arg_value, arg_name);
	args_free(args);
	return (false);
}

static inline void	args_load_flag(t_args *args, int *argc, char ***argv)
{
	if (*argc >= 1
		&& (*argv)[0][0] == '-'
		&& (*argv)[0][1] == 'l'
		&& (*argv)[0][2] == '\0')
	{
		args->custom_logs = true;
		(*argc)--;
		(*argv)++;
	}
}

bool	args_load(t_args *args, int argc, char **argv)
{
	const char	*name;

	name = argv[0];
	argc--;
	argv++;
	args_load_flag(args, &argc, &argv);
	if (argc < OPERAND_MIN || argc > OPERAND_MAX)
	{
		(void)printf("%s: Invalid usage: Expected %i to %i operands, got %i\n",
				name, OPERAND_MIN, OPERAND_MAX, argc);
		return (false);
	}
	if (args_parse_size_t(argv[0], &args->philo_count, false) == false)
		return (args_err(args, name, argv[ + 0], "number of philosophers"));
	if (args_parse_ms(argv[1], &args->time_to_die) == false)
		return (args_err(args, name, argv[1], "time to die"));
	if (args_parse_ms(argv[2], &args->time_to_eat) == false)
		return (args_err(args, name, argv[2], "time to eat"));
	if (args_parse_ms(argv[3], &args->time_to_sleep) == false)
		return (args_err(args, name, argv[3], "time to sleep"));
	if (argc < OPERAND_MAX)
		return (true);
	if (args_parse_size_t(argv[4], &args->meals_count, true) == false)
		return (args_err(args, name, argv[4], OPT_ARG_NAME));
	args->meals_is_set = true;
	return (true);
}

```

./srcs/4_args/args_priv.h
```c
#ifndef ARGS_PRIV_H
#define ARGS_PRIV_H

# include "types.h"
# include <stdbool.h>
# include <stddef.h>

bool	args_parse_size_t(const char *arg, size_t *out, bool accept_zero);
bool	args_parse_ms(const char *arg, t_ms *out);

#endif

```

./srcs/5_clock/1_clock_life_cycle.c
```c
#include "clock.h"
#include "mutex.h"
#include "run.h"

void	clock_init(t_clock *clock)
{
	clock->start_ms = UNSET_MS;
	clock->started = false;
	clock->stop = false;
	clock->error = false;
}

bool	clock_load(t_run *run)
{
	(void)run;
	return (true);
}

bool	clock_start(t_run *run, t_ms start_ms)
{
	t_clock	*clock;

	clock = &run->clock;
	if (mutex_lock(run, &run->mutex) == false)
		return (false);
	if (clock->started == true)
	{
		(void)run_stop_locked(run, true, "clock has already started");
		(void)mutex_unlock(run, &run->mutex);
		return (false);
	}
	clock->start_ms = start_ms;
	clock->started = true;
	if (mutex_unlock(run, &run->mutex) == false)
	{
		clock->stop = true;
		clock->error = true;
		return (false);
	}
	return (true);
}

void	clock_free(t_run *run)
{
	clock_init(&run->clock);
}

```

./srcs/5_clock/2_clock_is.c
```c
#include "clock.h"
#include "mutex.h"

bool	clock_is_started(t_run *run, bool *out)
{
	t_clock	*clock;

	clock = &run->clock;
	if (mutex_lock(run, &run->mutex) == false)
		return (false);
	*out = clock->started == true || clock->stop == true;
	return (mutex_unlock(run, &run->mutex));
}

bool	clock_is_stopped(t_run *run, bool mutex_is_locked, bool *out)
{
	if (mutex_is_locked == true)
		return (*out = run->clock.stop, true);
	if (mutex_lock(run, &run->mutex) == false)
		return (false);
	*out = run->clock.stop;
	return (mutex_unlock(run, &run->mutex));
}

bool	clock_is_error(t_run *run)
{
	t_clock	*clock;
	bool	res;

	clock = &run->clock;
	if (mutex_lock(run, &run->mutex) == false)
		return (false);
	res = clock->error;
	if (mutex_unlock(run, &run->mutex) == false)
	{
		clock->error = true;
		return (true);
	}
	return (res);
}

```

./srcs/5_clock/3_clock_get.c
```c
#include "clock.h"
#include "helpers.h"
#include "run.h"

bool	clock_get_start_ms(t_run *run, bool state_locked, t_ms *out)
{
	if (run->clock.start_ms == UNSET_MS)
	{
		if (state_locked == true)
			return (run_stop_locked(run, true, "clock has not started yet"));
		return (run_stop(run, true, "clock has not started yet"));
	}
	*out = run->clock.start_ms;
	return (true);
}

bool	clock_get_elapsed(t_run *run, bool state_locked, t_ms *out)
{
	t_ms	now;
	t_ms	start_ms;

	if (time_now(run, state_locked, &now) == false)
		return (false);
	if (clock_get_start_ms(run, state_locked, &start_ms) == false)
		return (false);
	*out = now - start_ms;
	return (true);
}

```

./srcs/6_forks/1_forks_life_cycle.c
```c
#include "forks.h"
#include "run.h"
#include <stdlib.h>
#include <stdint.h>

void	forks_init(t_forks *forks)
{
	forks->available = NULL;
	forks->count = UNSET_SIZE_T;
}

bool	forks_load(t_run *run)
{
	t_forks	*forks;
	size_t	i;

	forks = &run->forks;
	forks->count = run->args.philo_count;
	if (forks->count > SIZE_MAX / sizeof(*forks->available))
		return (run_stop(run, true, "forks: malloc size overflow"));
	forks->available = malloc(forks->count * sizeof(*forks->available));
	if (forks->available == NULL)
		return (run_stop(run, true, "forks: malloc() failed"));
	i = 0;
	while (i < forks->count)
		forks->available[i++] = true;
	return (true);
}

void	forks_free(t_run *run)
{
	if (run->forks.available != NULL)
		free(run->forks.available);
	forks_init(&run->forks);
}

```

./srcs/6_forks/2_forks_take_drop.c
```c
#include "forks.h"
#include "clock.h"
#include "mutex.h"
#include "helpers.h"
#include <limits.h>
#include <stdint.h>

static inline bool	forks_are_available(t_philo *philo)
{
	return (philo->last_meal <= philo->philo_left->last_meal
		&& philo->last_meal <= philo->philo_right->last_meal
		&& *philo->fork_left_is_available == true
		&& *philo->fork_right_is_available == true);
}

static inline bool	forks_try_take(t_philo *philo, bool *taken)
{
	*taken = false;

	if (forks_are_available(philo) == false)
		return (true);
	*philo->fork_left_is_available = false;
	*philo->fork_right_is_available = false;
	*taken = true;
	return (clock_get_elapsed(philo->run, true, &philo->last_meal));
}

bool	forks_take(t_philo *philo)
{
	bool	stopped;
	bool	taken;

	taken = false;
	while (true)
	{
		if (mutex_lock(philo->run, &philo->run->mutex) == false)
			return (false);
		if (!clock_is_stopped(philo->run, true, &stopped) || stopped == true)
			return ((void)mutex_unlock(philo->run, &philo->run->mutex), false);
		if (forks_try_take(philo, &taken) == false)
			return ((void)mutex_unlock(philo->run, &philo->run->mutex), false);
		if (mutex_unlock(philo->run, &philo->run->mutex) == false)
			return (false);
		if (taken == true)
			return (true);
		if (sleep_for_us(philo->run, SLEEP_DURATION_US) == false)
			return (false);
	}
}

bool	forks_drop(t_philo *philo)
{
	bool	stopped;

	if (mutex_lock(philo->run, &philo->run->mutex) == false)
		return (false);
	if (!clock_is_stopped(philo->run, true, &stopped) || stopped == true)
		return ((void)mutex_unlock(philo->run, &philo->run->mutex), false);
	*philo->fork_left_is_available = true;
	*philo->fork_right_is_available = true;
	philo->meal_count++;
	return (mutex_unlock(philo->run, &philo->run->mutex));
}

```

./srcs/7_philos/1_philo_life_cycle.c
```c
#include "philo_priv.h"
#include "thread.h"
#include <stdlib.h>

void	philo_init(t_philo *philo)
{
	philo->id = UNSET_SIZE_T;
	thread_init(&philo->thread);
	philo->last_meal = UNSET_MS;
	philo->meal_count = UNSET_SIZE_T;
	philo->fork_left_is_available = NULL;
	philo->fork_right_is_available = NULL;
	philo->philo_left = NULL;
	philo->philo_right = NULL;
	philo->run = NULL;
}

bool	philo_load(t_run *run, size_t index)
{
	t_philos	*philos;
	t_philo		*philo;
	size_t		right_fork_index;

	philos = &run->philos;
	philo = &philos->list[index];
	right_fork_index = (index + 1) % philos->count;
	philo->id = index + 1;
	philo->last_meal = 0;
	philo->meal_count = 0;
	philo->fork_left_is_available = &run->forks.available[index];
	philo->fork_right_is_available = &run->forks.available[right_fork_index];
	if (index == 0)
		philo->philo_left = &philos->list[philos->count - 1];
	else
		philo->philo_left = &philos->list[index - 1];
	philo->philo_right = &philos->list[(index + 1) % philos->count];
	philo->run = run;
	return (true);
}

void	philo_free(t_philo *philo)
{
	thread_free(philo->run, &philo->thread);
	philo_init(philo);
}

```

./srcs/7_philos/2_philos_life_cycle.c
```c
#include "philos.h"
#include "philo_priv.h"
#include "run.h"
#include <stdlib.h>
#include <stdint.h>

void	philos_init(t_philos *philos)
{
	philos->list = NULL;
	philos->count = UNSET_SIZE_T;
}

bool	philos_load(t_run *run)
{
	size_t	i;

	run->philos.count = run->args.philo_count;
	if (run->philos.count > SIZE_MAX / sizeof(*run->philos.list))
		return (run_stop(run, true, "philos: malloc size overflow"));
	run->philos.list = malloc(run->philos.count * sizeof(*run->philos.list));
	if (run->philos.list == NULL)
		return (run_stop(run, true, "philos: malloc() failed"));
	i = 0;
	while (i < run->philos.count)
	{
		philo_init(&run->philos.list[i]);
		if (philo_load(run, i) == false)
			return (philos_free(run, i), false);
		i++;
	}
	return (true);
}

void	philos_free(t_run *run, size_t count)
{
	size_t	i;

	if (run->philos.list != NULL)
	{
		i = 0;
		while (i < count)
			philo_free(&run->philos.list[i++]);
	}
	free(run->philos.list);
	philos_init(&run->philos);
}

```

./srcs/7_philos/3_philo_loops.c
```c
#include "philo_priv.h"
#include "clock.h"
#include "forks.h"
#include "helpers.h"
#include "logs.h"

bool	philo_eat(t_philo *philo)
{
	t_run	*run;
	t_ms	target;

	run = philo->run;
	if (forks_take(philo) == false)
		return (false);
	return (log_eat(philo)
		&& time_try_add(run, philo->last_meal, run->args.time_to_eat, &target)
		&& sleep_until(run, target)
		&& forks_drop(philo));
}

bool	philo_sleep(t_philo *philo)
{
	t_run	*run;
	t_ms	elapsed;
	t_ms	target;

	run = philo->run;
	return (clock_get_elapsed(run, false, &elapsed)
		&& log_sleep(philo, elapsed)
		&& time_try_add(run, elapsed, run->args.time_to_sleep, &target)
		&& sleep_until(run, target));
}

bool	philo_think(t_philo *philo)
{
	t_run	*run;
	t_ms	start;

	run = philo->run;
	return (clock_get_elapsed(run, false, &start) && log_think(philo, start));
}

```

./srcs/7_philos/3_philo_routine.c
```c
#include "philo_priv.h"
#include "logs.h"
#include "clock.h"
#include "helpers.h"
#include <stdio.h>

static inline bool	wait_for_start(t_philo *philo)
{
	bool	started;

	if (clock_is_started(philo->run, &started) == false)
		return (false);
	while (started == false)
	{
		if (sleep_for_us(philo->run, SLEEP_DURATION_US) == false)
			return (false);
		if (clock_is_started(philo->run, &started) == false)
			return (false);
	}
	if (philo->id % 2 == 0)
		return (true);
	return (sleep_for_ms(philo->run, philo->run->args.time_to_eat / 2));
}

static inline bool	wait_for_death(t_philo *philo)
{
	t_ms	elapsed;
	bool	stopped;

	if (clock_get_elapsed(philo->run, false, &elapsed) == false)
		return (false);
	if (log_fork(philo, elapsed) == false)
		return (false);
	while (true)
	{
		if (clock_is_stopped(philo->run, false, &stopped) == false)
			return (false);
		if (stopped == true)
			break ;
		if (sleep_for_us(philo->run, SLEEP_DURATION_US) == false)
			return (false);
	}
	return (true);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_ms	now;

	philo = arg;
	if (wait_for_start(philo) == false)
		return (NULL);
	if (philo->run->args.philo_count == 1)
		return ((void)wait_for_death(philo), NULL);
	while (true)
	{
		if (!philo_eat(philo) || !philo_sleep(philo) || !philo_think(philo))
			break ;
	}
	if (clock_get_elapsed(philo->run, false, &now))
		(void)log_stop(philo, now);
	return (NULL);
}

```

./srcs/7_philos/4_philos_start_stop.c
```c
#include "philos.h"
#include "philo_priv.h"
#include "thread.h"

bool	philos_start(t_run *run)
{
	size_t	i;
	t_philo	*philo;

	i = 0;
	while (i < run->args.philo_count)
	{
		philo = &run->philos.list[i];
		if (!thread_start(philo->run, &philo->thread, philo_routine, philo))
			return ((void)philos_stop(run, i), false);
		i++;
	}
	return (true);
}

bool	philos_stop(t_run *run, size_t count)
{
	size_t	i;
	t_philo	*philo;
	bool	success;

	success = true;
	i = 0;
	while (i < count)
	{
		philo = &run->philos.list[i];
		success = thread_stop(run, &philo->thread) && success == true;
		i++;
	}
	return (success);
}

```

./srcs/7_philos/philo_priv.h
```c
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

#endif

```

./srcs/8_run/1_run_life_cycle.c
```c
#include "run.h"
#include "logs.h"
#include "clock.h"
#include "forks.h"
#include "philos.h"
#include "args.h"
#include "mutex.h"

void	run_init(t_run *run)
{
	logs_init(&run->logs);
	mutex_init(&run->mutex);
	args_init(&run->args);
	clock_init(&run->clock);
	forks_init(&run->forks);
	philos_init(&run->philos);
}

bool	run_load(t_run *run, int argc, char **argv)
{
	if (logs_load(run, argv[0]) == false)
		return (false);
	if (mutex_load(run, &run->mutex, true) == false
		|| args_load(&run->args, argc, argv) == false
		|| clock_load(run) == false
		|| forks_load(run) == false
		|| philos_load(run) == false)
		return (run_free(run), false);
	return (true);
}

void	run_free(t_run *run)
{
	philos_free(run, run->args.philo_count);
	forks_free(run);
	clock_free(run);
	args_free(&run->args);
	mutex_free(run, &run->mutex, true);
	logs_free(run);
}

```

./srcs/8_run/2_run_start_stop.c
```c
#include "run.h"
#include "run_priv.h"
#include "philos.h"
#include "clock.h"
#include "helpers.h"
#include "logs.h"
#include "mutex.h"

bool	run_start(t_run *run)
{
	t_ms	now;

	if (run->args.meals_is_set == true && run->args.meals_count == 0)
		return (true);
	if (philos_start(run) == false)
		return (false);
	if (time_now(run, false, &now) == false
		|| clock_start(run, now) == false)
		return ((void)philos_stop(run, run->args.philo_count), false);
	return (run_monitor(run));
}

static bool	run_stop_safe(t_run *run, bool error, const char *message)
{
	run->clock.stop = true;
	run->clock.error |= error;
	if (error == true)
		return (log_error(run, message));
	return (logs_close(run));
}

bool	run_stop_locked(t_run *run, bool error, const char *message)
{
	return (run_stop_safe(run, error, message));
}

bool	run_stop(t_run *run, bool error, const char *message)
{
	bool	success;

	if (mutex_lock(run, &run->mutex) == false)
		return (false);
	success = run_stop_safe(run, error, message);
	return (mutex_unlock(run, &run->mutex) && success);
}

```

./srcs/8_run/3_run_monitor.c
```c
#include "run_priv.h"
#include "philos.h"
#include "clock.h"
#include "run.h"
#include "helpers.h"
#include "logs.h"
#include "mutex.h"

static inline bool	is_dead(t_run *run, t_philo *philo, t_ms now, bool *out)
{
	if (now >= run->args.time_to_die
		&& philo->last_meal <= now - run->args.time_to_die)
	{
		*out = true;
		run->clock.stop = true;
		return (log_death_locked(philo, now));
	}
	*out = false;
	return (true);
}

static inline bool	run_monitor_all_safe(t_run *run, bool *out_ended)
{
	t_ms	now;
	t_philo	*philo;
	bool	dead;
	size_t	i;

	if (run->clock.stop == true)
	{
		*out_ended = true;
		return (true);
	}
	if (clock_get_elapsed(run, true, &now) == false)
		return (false);
	*out_ended = run->args.meals_is_set;
	i = 0;
	while (i < run->philos.count)
	{
		philo = &run->philos.list[i++];
		if (is_dead(run, philo, now, &dead) == false)
			return (false);
		if (dead == true)
			return (*out_ended = true);
		*out_ended = *out_ended && philo->meal_count >= run->args.meals_count;
	}
	return (*out_ended == false || run_stop_locked(run, false, NULL));
}

static inline bool	run_monitor_all(t_run *run, bool *out_ended)
{
	bool	success;

	if (mutex_lock(run, &run->mutex) == false)
		return (false);
	success = run_monitor_all_safe(run, out_ended);
	return (mutex_unlock(run, &run->mutex) && success);
}

bool	run_monitor(t_run *run)
{
	bool	ended;

	while (true)
	{
		if (run_monitor_all(run, &ended) == false)
			return ((void)philos_stop(run, run->args.philo_count), false);
		if (ended == true)
			break ;
		if (sleep_for_us(run, SLEEP_DURATION_US) == false)
			return ((void)philos_stop(run, run->args.philo_count), false);
	}
	return (philos_stop(run, run->args.philo_count));
}

```

./srcs/8_run/4_run_panic.c
```c
#include "run.h"
#include "logs.h"

// Updates the stop state without locking because synchronization has failed.
bool	run_panic(t_run *run, const char *message)
{
	if (run != NULL)
	{
		run->logs.closed = true;
		run->logs.error = true;
		run->clock.stop = true;
		run->clock.error = true;
	}
	return (log_panic(run, message));
}

```

./srcs/8_run/run_priv.h
```c
#ifndef RUN_PRIV_H
# define RUN_PRIV_H

# include "types.h"

bool	run_monitor(t_run *run);

#endif

```

./srcs/main.c
```c
#include "run.h"
#include "logs.h"
#include <stdlib.h>

int	main(int argc, char **argv)
{
	t_run	run;

	run_init(&run);
	if (run_load(&run, argc, argv) == false)
		return (EXIT_FAILURE);
	if (run_start(&run) == false || run.clock.error == true)
		return (run_free(&run), EXIT_FAILURE);
	log_philos(&run);
	run_free(&run);
	return (EXIT_SUCCESS);
}

```

