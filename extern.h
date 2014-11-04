/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: extern.h,v 1.9 2002/05/05 11:09:44 oohara Exp $ */

#if !defined _EXTERN_H_
#define _EXTERN_H_

#ifdef HAVE_LIBSDL
#include <SDL.h>

#else /* not HAVE_LIBSDL */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif /* not HAVE_LIBSDL */

#ifdef EXTERN_DEF
 #define EXTERN 
#else
 #define EXTERN extern
#endif

/* wait by signal */
EXTERN int signal_delivered;
EXTERN int waittime;

#ifdef HAVE_LIBSDL
EXTERN SDL_Surface *dpy;
EXTERN SDL_Event event;

#else /* not HAVE_LIBSDL */

/* for XWindow */
EXTERN Display *dpy;
EXTERN Colormap cmap;
EXTERN Window   root;
EXTERN Window   win;
EXTERN Pixmap   WorkPixmap;
EXTERN XEvent   event;

EXTERN Font     font;
EXTERN GC       FontGC;
EXTERN GC       BackGC;
EXTERN GC       FillGC; /* GC for debugging rectangles */
EXTERN XColor   black;
EXTERN XColor   white;

#endif /* not HAVE_LIBSDL */


EXTERN int      keymask;
EXTERN int      joymask;

EXTERN char *upKey;
EXTERN char *downKey;
EXTERN char *leftKey;
EXTERN char *rightKey;
EXTERN char *shotKey;
EXTERN char *spdupKey;
EXTERN char *spdwnKey;
EXTERN char *pauseKey;
EXTERN char *quitKey;
#ifdef DEBUG
EXTERN char *weaponchangeKey;
EXTERN char *clearenemyshotKey;
#endif /* DEBUG */

/* character management */
EXTERN CharManage *manage;

/* player data (score.. stage...) */
EXTERN PlayerData *player;

/* field */
EXTERN int FieldW;
EXTERN int FieldH;

/* star */
EXTERN int StarPtn1;
EXTERN int StarPtn2;

/* image */
EXTERN Image **PlayerImage;
EXTERN Image **PShot1Image;
EXTERN Image **PShot2Image;
EXTERN Image **PShot3Image;

EXTERN Image **EShotImage;
EXTERN Image **ELaserImage;
EXTERN Image **EMissileImage;
EXTERN Image **EBoundImage;
EXTERN Image **ERingImage;

EXTERN Image **BombImage;
EXTERN Image **LargeBombImage;

EXTERN Image **Enemy1Image;
EXTERN Image **Enemy2Image;
EXTERN Image **Enemy3Image;
EXTERN Image **Enemy4Image;
EXTERN Image **Enemy5Image;
EXTERN Image **Enemy6Image;
EXTERN Image **Enemy7Image;

EXTERN Image **Boss1Image;
EXTERN Image **Boss2Image;
EXTERN Image **Boss3Image;
EXTERN Image **Boss4Image;
EXTERN Image **Boss5Image;
EXTERN Image **Boss6Image;
EXTERN Image **Boss7Image;

EXTERN Image **ItemImage;

EXTERN char display[32];

#endif
