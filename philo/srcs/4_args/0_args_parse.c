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
