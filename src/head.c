#include <err.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

static void
head(const char *fname, FILE *f, size_t n)
{
	size_t i, bsize;
 	ssize_t len;
	char *buf;

	buf   = NULL;
	bsize = 0;
	i     = 0;

	for (; i < n; i++) {
		if ((len = getline(&buf, &bsize, f)) < 0)
			break;

		fwrite(buf, sizeof(char), len, stdout);
	}

	free(buf);
	buf = NULL;

	if (ferror(f))
		err(1, "getline %s", fname);
}

static void
usage(void)
{
	fprintf(stderr, "usage: %s [-n number] [file ...]\n", getprogname());
	exit(1);
}

int
main(int argc, char *argv[])
{
	FILE *fp;
	size_t n;
	int first, rval;

	first =  1;
	n     = 10;
	rval  =  0;
	setprogname(argv[0]);

	ARGBEGIN {
	case 'n':
		n = stoll(EARGF(usage()), 0, LLONG_MAX);
		break;
	default:
		usage();
	} ARGEND

	if (!argc)
		head("<stdin>", stdin, n);

	for (; *argv; argv++) {
		if (ISDASH(*argv)) {
			*argv = "<stdin>";
			fp = stdin;
		} else if (!(fp = fopen(*argv, "r"))) {
			warn("fopen %s", *argv);
			rval = 1;
			continue;
		}

		if (argc > 1) {
			printf("%s==> %s <==\n", first ? "" : "\n", *argv);
			first = 0;
		}

		head(*argv, fp, n);

		if (fp != stdin)
			fclose(fp);
	}

	exit(rval);
}
