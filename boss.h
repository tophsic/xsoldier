/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: boss.h,v 1.2 2002/04/10 07:38:12 oohara Exp $ */

#if !defined _BOSS_H_
#define _BOSS_H_

extern int NewBoss1(void);
extern DelAtt BossAct1(ObjData *my);
extern DelAtt BossHit1(ObjData *my, ObjData *your);

extern int NewBoss2(void);
extern DelAtt BossAct2(ObjData *my);

extern int NewBoss3(void);
extern DelAtt BossAct3(ObjData *my);

extern int NewBoss4(void);
extern DelAtt BossAct4(ObjData *my);

extern int NewBoss5(void);
extern DelAtt BossAct5(ObjData *my);

extern int NewBoss6(void);
extern DelAtt BossAct6(ObjData *my);

extern int NewBoss7(void);
extern DelAtt BossAct7(ObjData *my);

extern int NewBoss8(void);
extern DelAtt BossAct8(ObjData *my);
extern DelAtt BossHit8(ObjData *my, ObjData *your);

#endif
