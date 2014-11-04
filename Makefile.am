## Makefile.am for xsoldier
## process this file with automake --foreign to generate Makefile.in
## automake 1.11 or later is required
# $Id: Makefile.am,v 1.27 2011/08/10 14:33:08 oohara Exp $

bin_PROGRAMS = xsoldier
xsoldier_SOURCES = boss.c \
  callback.c \
  common.c \
  ending.c \
  enemy.c \
  enemyshot.c \
  game.c \
  graphic.c \
  image.c \
  input.c \
  joystick.c \
  main.c \
  manage.c \
  opening.c \
  player.c \
  score.c \
  sin.c \
  star.c \
  boss.h \
  callback.h \
  common.h \
  ending.h \
  enemy.h \
  enemyshot.h \
  extern.h \
  game.h \
  graphic.h \
  image.h \
  input.h \
  joystick.h \
  key.h \
  manage.h \
  opening.h \
  player.h \
  score.h \
  sin.h \
  star.h \
  version.h \
  xsoldier.h

man_MANS = xsoldier.6

EXTRA_DIST = GPL \
  LICENSE \
  old-doc \
  pixmap \
  scorefile.txt \
  xsoldier.6

#CC_WARNINGS = -Wall
# the overkill warnings
# use -Wno-long-long because -pedantic implies -Wlong-long
# don't use -Wcast-qual because the file name for XpmReadFileToPixmap is
# char *, not const char *
CC_WARNINGS = -Wall -Wshadow -Wpointer-arith \
  -Wcast-align -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes \
  -Wmissing-declarations -Wnested-externs -Winline -pedantic -Wno-long-long
AM_CPPFLAGS = -DDATADIR=\"$(datadir)\" \
  -DLOCALSTATEDIR=\"$(localstatedir)\"
# -g -O2 are in CFLAGS by default, don't use @CFLAGS@ here
AM_CFLAGS = $(X_CFLAGS) $(SDL_CFLAGS) $(CC_WARNINGS)

install-exec-local:
# install pixmap
	$(mkinstalldirs) $(DESTDIR)$(datadir)/games/xsoldier/
	$(INSTALL_DATA) pixmap/*.xpm \
  $(DESTDIR)$(datadir)/games/xsoldier/
# install score file
	$(mkinstalldirs) $(DESTDIR)$(localstatedir)/games/xsoldier/
	test -e $(DESTDIR)$(localstatedir)/games/xsoldier/xsoldier.scores \
  || $(INSTALL_DATA) scorefile.txt \
  $(DESTDIR)$(localstatedir)/games/xsoldier/xsoldier.scores

uninstall-local:
# uninstall pixmap
	-rm -f $(DESTDIR)$(datadir)/games/xsoldier/*.xpm
	-rmdir $(DESTDIR)$(datadir)/games/xsoldier/
# uninstall score file
	-rm -f $(DESTDIR)$(localstatedir)/games/xsoldier/xsoldier.scores
	-rmdir $(DESTDIR)$(localstatedir)/games/xsoldier/
