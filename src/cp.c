/* This file is part of the UtilChest from Draco Project
 * See LICENSE file for copyright and license details.
 */
#include <sys/stat.h>

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fs.h"
#include "util.h"

static const char *usage = "[-afp] [-R [-H|-L|-P]] source ... dest";

int
main(int argc, char *argv[]) {
	const char *sourcedir;
	int (*cp)(const char *, const char *, int) = copy_file;
	int rval = 0, opts = 0;
	struct stat st;

	ARGBEGIN {
	case 'a':
		cp = copy_folder;
		ftr_follow = 'P';
		opts |= CP_P;
		break;
	case 'f':
		opts |= CP_F;
		break;
	case 'p':
		opts |= CP_P;
		break;
	case 'r':
	case 'R':
		cp = copy_folder;
		break;
	case 'H':
	case 'L':
	case 'P':
		ftr_follow = ARGC();
		break;
	default:
		wrong(usage);
	} ARGEND

	switch (argc) {
	case 0:
	case 1:
		wrong(usage);
	case 2:
		exit(cp(argv[0], argv[1], opts));
	}

	sourcedir = argv[argc - 1];
	if (stat(sourcedir, &st) < 0)
		perr(1, "stat %s:", sourcedir);

	if (!S_ISDIR(st.st_mode))
		wrong(usage);

	for (; *argv != sourcedir; argc--, argv++)
		rval |= cp(*argv, sourcedir, opts);

	return rval;
}
