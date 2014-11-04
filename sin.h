/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: sin.h,v 1.2 2002/04/07 13:42:02 oohara Exp $ */

#if !defined _SIN_H_
#define _SIN_H_

double dsin(int theta);
int isin(int theta);
#define dcos(i) dsin(i+90)
#define icos(i) isin(i+90)

#endif
