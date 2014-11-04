/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: manage.h,v 1.2 2002/04/12 00:30:37 oohara Exp $ */

#if !defined _MANAGE_H_
#define _MANAGE_H_

extern CharManage *NewManage(int playerMax, int enemyMax);
extern void ClearManage(CharManage *manage_temp);
extern void ResetManage(CharManage *manage_temp);
extern void DeleteManage(CharManage *Del);

extern int NewObj(int mask,
		  DelAtt (*action)(ObjData *my),
		  DelAtt (*hit)(ObjData *my, ObjData *your),
		  void (*realize)(ObjData *my, GrpData *grp));
extern void DelObj(CharObj *del);
extern PlayerData *NewPlayerData(void);
extern void ClearEnemyShotManage(CharManage *manage_temp);

#endif
