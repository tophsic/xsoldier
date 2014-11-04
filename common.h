/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: common.h,v 1.2 2002/04/20 16:39:19 oohara Exp $ */

#if !defined _COMMON_H_
#define _COMMON_H_

extern void NewBomb(int x, int y);
extern void NewLargeBomb(int x, int y);
extern DelAtt BombAct(ObjData *my);
extern int GetDirection(int mx, int my, int sx, int sy);

#endif
