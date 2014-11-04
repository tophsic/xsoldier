/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: star.h,v 1.3 2002/04/20 00:19:14 oohara Exp $ */

#ifndef STAR_H_INCLUDE
#define STAR_H_INCLUDE

extern void InitStarModule(unsigned int w, unsigned int h);
extern int CreateStar(const char *filename, int pattern, int speed, int nstar);
extern void DrawStar(int id);
extern void ChangeStarParameter(int id, int speed);
extern int DeleteAllStar(void);

#endif /* STAR_H_INCLUDE */
