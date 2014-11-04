/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: graphic.h,v 1.8 2002/04/22 13:45:34 oohara Exp $ */

#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_
extern int graphic_init(void);
extern int clear_window(void);
extern int redraw_window(void);
extern int graphic_finish(void);
extern int draw_string(int x, int y, const char *string, int length);
extern int draw_char(int x, int y, int c);

#ifdef HAVE_LIBSDL
extern int sdl_draw_rect(int x_src, int y_src, int w, int h);
#endif /* HAVE_LIBSDL */

#endif /* _GRAPHIC_H_ */
