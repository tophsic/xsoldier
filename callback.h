/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: callback.h,v 1.2 2002/04/10 07:46:25 oohara Exp $ */

#if !defined _CALLBACK_H_
#define _CALLBACK_H_

extern DelAtt NullAct(ObjData *my);

extern DelAtt NullHit(ObjData *my, ObjData *your);
extern DelAtt NullDelHit(ObjData *my, ObjData *your);
extern DelAtt DeleteHit(ObjData *my, ObjData *your);
extern DelAtt DamageHit(ObjData *my, ObjData *your);
extern DelAtt LargeDamageHit(ObjData *my, ObjData *your);

extern void NullReal(ObjData *my, GrpData *grp);
extern void DrawRec(ObjData *my, GrpData *grp);
extern void DrawImage(ObjData *my, GrpData *grp);

#endif
