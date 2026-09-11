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
