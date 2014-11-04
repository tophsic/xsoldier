/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: key.h,v 1.5 2002/04/21 19:16:39 oohara Exp $ */

#include <X11/keysym.h>

#if !defined _KEY_H_
#define _KEY_H_

/* definitions of keys */

#ifdef HAVE_LIBSDL
#define KeySym SDLKey
#define UpKey    SDLK_UP
#define DownKey  SDLK_DOWN
#define LeftKey  SDLK_LEFT
#define RightKey SDLK_RIGHT

#define ShotKey  SDLK_LSHIFT

#define SpeedUPKey   SDLK_a
#define SpeedDOWNKey SDLK_s

#define PauseKey      SDLK_p
#define QuitKey      SDLK_q

#ifdef DEBUG
#define WeaponChangeKey SDLK_w
#define ClearEnemyShotKey SDLK_c
#endif /* DEBUG */

#else /* not HAVE_LIBSDL */
#define UpKey    XK_Up
#define DownKey  XK_Down
#define LeftKey  XK_Left
#define RightKey XK_Right

#define ShotKey  XK_Shift_L

#define SpeedUPKey   XK_a
#define SpeedDOWNKey XK_s

#define PauseKey      XK_p
#define QuitKey      XK_q

#ifdef DEBUG
#define WeaponChangeKey XK_w
#define ClearEnemyShotKey XK_c
#endif /* DEBUG */

#endif /* not HAVE_LIBSDL */


/* don't touch them */
#define Up    (1L<<0)
#define Down  (1L<<1)
#define Left  (1L<<2)
#define Right (1L<<3)
#define Shot  (1L<<4)
#define SpeedUP   (1L<<5)
#define SpeedDOWN (1L<<6)
#define Pause (1L<<7)
#define Quit (1L<<8)

#endif
