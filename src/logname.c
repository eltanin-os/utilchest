#include <err.h>
#include <stdio.h>
#include <unistd.h>

int
main(void)
{
	const char *logname;

	if (!(logname = getlogin()))
		err(1, "getlogin");
	puts(logname);

	return 0;
}
