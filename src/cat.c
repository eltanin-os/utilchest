/* This file is part of the UtilChest from Draco Project
 * See LICENSE file for copyright and license details.
 */
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "util.h"

static const char *usage = "[-u] [file ...]";

static int
cat(int f, const char *name) {
	char buf[BUFSIZ];
	ssize_t n;

	while ((n = read(f, buf, sizeof(buf))) > 0)
		if (write(1, buf, n) != n)
			return (pwarn("write %s:", name));

	if (n < 0)
		return (pwarn("read %s:", name));

	return 0;
}

int
main(int argc, char *argv[]) {
	int f, rval = 0;

	ARGBEGIN {
	case 'u':
		break;
	default:
		wrong(usage);
	} ARGEND

	if (!argc)
		cat(0, "<stdin>");

	for (; *argv; argv++) {
		if ((f = open(*argv, O_RDONLY, 0)) < 0) {
			rval = pwarn("open %s:", *argv);
			continue;
		}

		rval |= cat(f, *argv);

		if (f != -1 && close(f) < 0)
			rval = pwarn("close %s:", *argv);
	}

	return rval;
}
