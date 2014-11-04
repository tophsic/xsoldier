/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: score.h,v 1.3 2002/04/07 13:49:04 oohara Exp $ */

#if !defined _SCORE_H_
#define _SCORE_H_

extern int ReadHiscore(PlayerData *p);
extern int MergeHiscore(PlayerData *p);
extern int WriteHiscore(PlayerData *p);

#endif
