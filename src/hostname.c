#include <err.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

static void
usage(void)
{
	fprintf(stderr, "usage: %s [name]\n", getprogname());
	exit(1);
}

int
main(int argc, char *argv[])
{
	char host[HOST_NAME_MAX+1];

	setprogname(argv[0]);
	argc--, argv++;

	if (argc > 1)
		usage();

	if (argc == 1) {
		if (sethostname(*argv, strlen(*argv)))
			err(1, "sethostname");
	} else {
		if (gethostname(host, sizeof(host)))
			err(1, "gethostname");

		puts(host);
	}

	return (ioshut());
}
