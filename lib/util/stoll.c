#include <err.h>
#include <errno.h>
#include <stdlib.h>

#include "util.h"

long long
stoll(const char *str, long long min, long long max)
{
	long long ll;
	char *end;

	errno = 0;
	ll    = strtoll(str, &end, 10);

	if (end == str || *end != '\0')
		errno = EINVAL;

	if (ll > max || ll < min)
		errno = ERANGE;

	if (errno)
		err(1, "stoll %s", str);

	return ll;
}
