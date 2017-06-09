/* This file is part of the UtilChest from Draco Project
 * See LICENSE file for copyright and license details.
 */
extern char *argv0;

#define ARGBEGIN \
for (argv0 = *argv, argc--, argv++; argv[0] && argv[0][0] == '-' && argv[0][1]; argc--, argv++) {\
	char _argc;\
	for (argv[0]++; argv[0][0]; argv[0]++) {\
		_argc = argv[0][0];\
		switch (_argc)

#define ARGEND } }

#define ARGC() _argc;
