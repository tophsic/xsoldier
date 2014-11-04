dnl configure.ac for xsoldier
dnl Process this file with autoconf to produce a configure script.
dnl autoconf 2.64 or later is required
# $Id: configure.ac,v 1.27 2011/08/12 14:30:46 oohara Exp $
AC_INIT([xsoldier], [1.8], [oohara@libra.interq.or.jp])
AC_PREREQ([2.64])
AC_REVISION([$Revision: 1.27 $])
AM_INIT_AUTOMAKE([foreign tar-ustar])
AC_CONFIG_SRCDIR([star.h])
dnl don't quote config.h with []
AM_CONFIG_HEADER(config.h)

AC_ARG_VAR([SDL_CFLAGS], [gcc flags for SDL library])
AC_ARG_VAR([SDL_LIBS], [ld flags for SDL library])
AC_ARG_VAR([MY_X_LIBS], [ld flags for X library])

# Checks for programs.
# awk is used in Makefile generated from Makefile.am
AC_PROG_AWK
AC_PROG_CC
# cpp is required by AC_CHECK_HEADERS
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S

# Checks for libraries.
# FIXME: -lX11
#AC_CHECK_LIB([X11], [XOpenDisplay])
# FIXME: -lXpm
#AC_CHECK_LIB([Xpm], [XpmReadFileToPixmap])

# --with support
# sdl
# we don't do AC_CHECK_LIB([SDL], [SDL_Init]) here because it fails on
# a non-i386 architecture (perhaps because of missing -lpthread?)
#
# don't modify LIBS here, or checks for library functions will fail
# even if the functions do exist
AC_ARG_WITH([sdl],
  AC_HELP_STRING([--with-sdl], [use SDL instead of X for input / output]),
  ,
  [with_sdl=no])

if test "x$with_sdl" = "xyes"
then
  AC_DEFINE(HAVE_LIBSDL, , [Define if you have the `SDL' library (-lSDL).])
  SDL_CFLAGS=`sdl-config --cflags`
  SDL_LIBS="-lSDL_image `sdl-config --libs`"
else
  AC_PATH_XTRA
  MY_X_LIBS="-lX11 -lXpm"
fi

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([errno.h fcntl.h malloc.h stdlib.h string.h sys/ioctl.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.
# for ioctl in joystick.c
AC_PROG_GCC_TRADITIONAL
AC_FUNC_MALLOC
AC_CHECK_FUNCS([atexit memset strrchr rand])

# the first argument of AC_DEFINE must be literal, the third is mandatory
# --enable support

# joystick
AC_ARG_ENABLE([joystick],
  AC_HELP_STRING([--enable-joystick], [use joystick to move player (unmaintained)]),
  ,
  [enable_joystick=no])

if test "x$enable_joystick" = "xyes"
then
  AC_DEFINE(JSTK, , [Define to enable unmaintained joystick support.])
fi

# debug
AC_ARG_ENABLE([debug],
  AC_HELP_STRING([--enable-debug], [enable additional debugging features]),
  ,
  [enable_debug=no])

if test "x$enable_debug" = "xyes"
then
  AC_DEFINE(DEBUG, , [Define to enable debugging.])
fi

# duplicated entries in the high score list
AC_ARG_ENABLE([duplicated-scores],
  AC_HELP_STRING([--enable-duplicated-scores], [allow the same person to have 2 or more entries in the high score list]),
  ,
  [enable_duplicated_scores=no])

if test "x$enable_duplicated_scores" = "xyes"
then
  AC_DEFINE(DUPSCORE, , [Define to allow duplicated entries in the high score list])
fi

# setting these variable in Makefile.am is the bad thing
AC_SUBST([LDFLAGS], ["$X_LIBS $LDFLAGS"])
AC_SUBST([LIBS], ["$X_PRE_LIBS $MY_X_LIBS $SDL_LIBS $LIBS"])
AC_SUBST([SDL_CFLAGS])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
