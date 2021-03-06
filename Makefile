include config.mk

.SUFFIXES:
.SUFFIXES: .o .c

INC= inc

HDR=\
	inc/arg.h\
	inc/compat.h\
	inc/crypto.h\
	inc/utf.h\
	inc/util.h

# SOURCE
BIN=\
	src/basename\
	src/cat\
	src/chgrp\
	src/chmod\
	src/chown\
	src/chroot\
	src/cksum\
	src/cmp\
	src/cp\
	src/date\
	src/dirname\
	src/domainname\
	src/du\
	src/echo\
	src/env\
	src/false\
	src/flock\
	src/head\
	src/hostname\
	src/id\
	src/install\
	src/kill\
	src/link\
	src/ln\
	src/logger\
	src/logname\
	src/ls\
	src/mkdir\
	src/mkfifo\
	src/mknod\
	src/mktemp\
	src/mv\
	src/nice\
	src/nohup\
	src/pathchk\
	src/printenv\
	src/pwd\
	src/readlink\
	src/renice\
	src/rev\
	src/rm\
	src/rmdir\
	src/setsid\
	src/sha1sum\
	src/sha224sum\
	src/sha256sum\
	src/sha512sum\
	src/sleep\
	src/sync\
	src/tee\
	src/time\
	src/touch\
	src/true\
	src/tty\
	src/uname\
	src/unlink\
	src/which\
	src/whoami\
	src/yes

MAN=\
	man/basename.1\
	man/cat.1\
	man/chgrp.1\
	man/chmod.1\
	man/chown.1\
	man/chroot.1\
	man/cksum.1\
	man/cmp.1\
	man/cp.1\
	man/date.1\
	man/dirname.1\
	man/domainname.1\
	man/du.1\
	man/echo.1\
	man/env.1\
	man/false.1\
	man/flock.1\
	man/head.1\
	man/hostname.1\
	man/id.1\
	man/install.1\
	man/kill.1\
	man/link.1\
	man/ln.1\
	man/logger.1\
	man/logname.1\
	man/ls.1\
	man/mkdir.1\
	man/mkfifo.1\
	man/mknod.1\
	man/mktemp.1\
	man/mv.1\
	man/nice.1\
	man/nohup.1\
	man/pathchk.1\
	man/printenv.1\
	man/pwd.1\
	man/readlink.1\
	man/renice.1\
	man/rev.1\
	man/rm.1\
	man/rmdir.1\
	man/setsid.1\
	man/sha1sum.1\
	man/sha224sum.1\
	man/sha256sum.1\
	man/sha512sum.1\
	man/sleep.1\
	man/sync.1\
	man/tee.1\
	man/time.1\
	man/touch.1\
	man/true.1\
	man/tty.1\
	man/uname.1\
	man/unlink.1\
	man/which.1\
	man/whoami.1\
	man/yes.1

# LIB SOURCE
LIBUTFSRC=\
	lib/utf/chartorune.c\
	lib/utf/iscntrlrune.c\
	lib/utf/isprintrune.c\
	lib/utf/isvalidrune.c\
	lib/utf/runetype.c

LIBUTILSRC=\
	lib/util/chmod.c\
	lib/util/chown.c\
	lib/util/concat.c\
	lib/util/cp.c\
	lib/util/crypto.c\
	lib/util/dir.c\
	lib/util/ealloc.c\
	lib/util/fshut.c\
	lib/util/fgetline.c\
	lib/util/genpath.c\
	lib/util/mode.c\
	lib/util/pathcat.c\
	lib/util/sha1.c\
	lib/util/sha224.c\
	lib/util/sha256.c\
	lib/util/sha512.c\
	lib/util/strtobase.c

# LIB PATH
LIBUTF=    lib/libutf.a
LIBUTIL=   lib/libutil.a

# LIB OBJS
LIBUTFOBJ=    $(LIBUTFSRC:.c=.o)
LIBUTILOBJ=   $(LIBUTILSRC:.c=.o)

# ALL
LIB= $(LIBUTIL) $(LIBUTF)
OBJ= $(BIN:=.o) $(LIBUTILOBJ) $(LIBUTFOBJ)
SRC= $(BIN:=.c)

# VAR RULES
all: $(BIN)

$(BIN): $(LIB) $(@:=.o)
$(OBJ): $(HDR) config.mk

# SUFFIX RULES
.o:
	$(CC) $(LDFLAGS) -o $@ $< $(LIB)

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -I $(INC) -o $@ -c $<

# LIBRARIES RULES
$(LIBUTF): $(LIBUTFOBJ)
	$(AR) rc $@ $?
	$(RANLIB) $@

$(LIBUTIL): $(LIBUTILOBJ)
	$(AR) rc $@ $?
	$(RANLIB) $@

# USER ACTIONS
utilchest: $(LIB) $(SRC)
	mkdir -p build
	for f in $(SRC); do sed "s/^main(/$$(echo "$$(basename $${f%.c})" | sed s/-/_/g)_&/" < $$f > build/$$(basename $$f); done
	echo '#include <libgen.h>'                                                                                                                              > build/$@.c
	echo '#include <stdio.h>'                                                                                                                               >> build/$@.c
	echo '#include <string.h>'                                                                                                                              >> build/$@.c
	for f in $(SRC); do echo "int $$(echo "$$(basename $${f%.c})" | sed s/-/_/g)_main(int, char **);"; done                                                 >> build/$@.c
	echo 'int main(int argc, char *argv[]) { char *s = basename(argv[0]);'                                                                                  >> build/$@.c
	echo 'if(!strcmp(s,"utilchest")) { argc--; argv++; s = basename(argv[0]); } if(0) ;'                                                                    >> build/$@.c
	for f in $(SRC); do echo "else if(!strcmp(s, \"$$(basename $${f%.c})\")) return $$(echo "$$(basename $${f%.c})" | sed s/-/_/g)_main(argc, argv);"; done >> build/$@.c
	echo 'else { '                                                                                                                                          >> build/$@.c
	for f in $(SRC); do echo "fputs(\"$$(basename $${f%.c}) \", stdout);"; done                                                                             >> build/$@.c
	echo 'putchar(0xa); }; return 0; }'                                                                                                                     >> build/$@.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -I $(INC) -o $@ build/*.c $(LIB)
	rm -rf build

install-utilchest: utilchest
	install -dm 755 $(DESTDIR)/$(PREFIX)/bin
	install -csm 755 utilchest $(DESTDIR)/$(PREFIX)/bin
	for f in $$(echo $(BIN) | sed 's/src\///g'); do ln -s utilchest $(DESTDIR)/$(PREFIX)/bin/$$f; done

install: all
	install -dm 755 $(DESTDIR)/$(PREFIX)/bin
	install -csm 755 $(BIN) $(DESTDIR)/$(PREFIX)/bin

install-man:
	install -dm 755 $(DESTDIR)/$(MANPREFIX)/man1
	install -cm 644 $(MAN) $(DESTDIR)/$(MANPREFIX)/man1

clean:
	rm -f $(BIN) $(OBJ) $(LIB) utilchest

.PHONY:
	all clean install install-man install-utilchest utilchest

