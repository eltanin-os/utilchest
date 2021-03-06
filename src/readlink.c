#include <err.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

static void
usage(void)
{
	fprintf(stderr, "usage: %s [-fn] file\n", getprogname());
	exit(1);
}

int
main(int argc, char *argv[])
{
	ssize_t n;
	int fflag, nflag;
	char buf[PATH_MAX];

	fflag = 0;
	nflag = 0;
	setprogname(argv[0]);

	ARGBEGIN {
	case 'f':
		fflag = 1;
		break;
	case 'n':
		nflag = 1;
		break;
	default:
		usage();
	} ARGEND

	if (argc != 1)
		usage();

	if (strlen(*argv) >= PATH_MAX) {
		errno = ENAMETOOLONG;
		err(1, NULL);
	}

	if (fflag) {
		if (!(realpath(*argv, buf)))
			err(1, "realpath %s", *argv);
	} else {
		if ((n = readlink(*argv, buf, PATH_MAX - 1)) < 0)
			exit(1);
		buf[n] = '\0';
	}

	fputs(buf, stdout);

	if (!nflag)
		putchar('\n');

	return (ioshut());
}
