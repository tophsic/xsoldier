/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: enemy.h,v 1.2 2002/04/12 00:03:57 oohara Exp $ */

#if !defined _ENEMY_H_
#define _ENEMY_H_

extern int NewEnemy1(int x, int y);
extern DelAtt EnemyAct1(ObjData *my);
extern DelAtt EnemyHit1(ObjData *my, ObjData *your);

extern int NewEnemy2(int x, int y);
extern DelAtt EnemyAct2(ObjData *my);
extern int NewEnemy3(int x, int y);
extern DelAtt EnemyAct3(ObjData *my);
extern int NewEnemy4(int x, int y);
extern DelAtt EnemyAct4(ObjData *my);
extern int NewEnemy5(int x, int y);
extern DelAtt EnemyAct5(ObjData *my);
extern int NewEnemy6(int x, int y);
extern DelAtt EnemyAct6(ObjData *my);
extern int NewEnemy7(int x, int y);
extern DelAtt EnemyAct7(ObjData *my);
extern int NewEnemy8(int x, int y);
extern DelAtt EnemyAct8(ObjData *my);
extern int NewEnemy9(int x, int y);
extern DelAtt EnemyAct9(ObjData *my);
extern int NewEnemy10(int x, int y);
extern DelAtt EnemyAct10(ObjData *my);

#endif
