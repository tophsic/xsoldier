/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: player.h,v 1.3 2002/04/16 06:11:18 oohara Exp $ */

#if !defined _PLAYER_H_
#define _PLAYER_H_

extern void NewPlayer(int x, int y);
extern void RestartPlayer(int x, int y);
extern DelAtt PlayerAction(ObjData *my);
extern DelAtt PlayerHit(ObjData *my, ObjData *your);

extern void PlayerShot1(int x, int y, int speed, int angle, int attack);
extern void PlayerShot2(int x, int y, int speed, int angle);
extern DelAtt PlayerShotAct1(ObjData *my);
extern DelAtt PlayerShotHit1(ObjData *my, ObjData *your);
extern void PlayerShot3(int x, int y, int inertX, int attack);
extern DelAtt PlayerShotAct3(ObjData *my);
extern DelAtt PlayerShotHit3(ObjData *my, ObjData *your);
extern void PlayerLosePower(void);

#endif
