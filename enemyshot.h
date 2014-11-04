/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: enemyshot.h,v 1.2 2002/04/12 00:05:11 oohara Exp $ */

#if !defined _ENEMYSHOT_H_
#define _ENEMYSHOT_H_

extern void ShotToAngle(int x, int y, int angle, int speed);
extern void ShotToPoint(int x1, int y1, int x2, int y2, int speed);
extern DelAtt EnemyShotAct(ObjData *my);
extern int RingToAngle(int x, int y, int angle, int speed);
extern int RingToPoint(int x1, int y1, int x2, int y2, int speed);
extern int HomingShot(int x, int y, int ix, int iy);
extern DelAtt HomingAct(ObjData *my);
extern int LaserShot(int x, int y, int speed);
extern DelAtt EnemyLaserAct(ObjData *my);
extern int BoundShot(int x, int y, int ix, int iy, int bound);
extern DelAtt BoundShotAct(ObjData *my);

#endif
