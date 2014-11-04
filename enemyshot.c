/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: enemyshot.c,v 1.7 2002/04/29 16:54:59 oohara Exp $ */

#include <stdio.h>
#include <stdlib.h>

/*
#include <X11/Xlib.h>
#include <X11/xpm.h>
*/

#include "image.h"
#include "xsoldier.h"
#include "manage.h"
#include "common.h"
#include "enemyshot.h"
#include "callback.h"
#include "extern.h"
#include "sin.h"

/* enemy shot */
void ShotToAngle(int x, int y, int angle, int speed)
{
    int i;

    if (manage->EnemyNum >= manage->EnemyMax)
        return;

    if (speed <= 0)
      speed = 1;
    
    for (i=1; i<manage->EnemyMax; i++)
    {
        if (manage->enemy[i]->Data.used == False)
	{
	    manage->EnemyShot.Data.X = x;
	    manage->EnemyShot.Data.Y = y;
	    manage->EnemyShot.Data.Angle = angle;
	    manage->EnemyShot.Data.Speed = speed;
	    manage->EnemyShot.Data.Cnt[0] = x << 8;
	    manage->EnemyShot.Data.Cnt[1] = y << 8;
	    manage->EnemyShot.Data.Cnt[2] = icos(angle);
	    manage->EnemyShot.Data.Cnt[3] = isin(angle);

	    manage->enemy[i]->Data    = manage->EnemyShot.Data;
	    manage->enemy[i]->Grp     = manage->EnemyShot.Grp;
            manage->enemy[i]->Action  = EnemyShotAct;
            manage->enemy[i]->Realize = DrawImage;
            manage->enemy[i]->Hit     = NullDelHit;

	    manage->EnemyNum++;
	    return;
	}
    }
}

void ShotToPoint(int x1, int y1, int x2, int y2, int speed)
{
    int i;
    int diffx = x2 - x1;
    int diffy = y2 - y1;
    int absx = abs(diffx);
    int absy = abs(diffy);

    if (manage->EnemyNum >= manage->EnemyMax)
        return;

    if (speed <= 0)
      speed = 1;
    
    for (i=1; i<manage->EnemyMax; i++)
    {
        if (manage->enemy[i]->Data.used == False)
	{
	    manage->EnemyShot.Data.X = x1;
	    manage->EnemyShot.Data.Y = y1;
	    manage->EnemyShot.Data.Speed = speed;
	    manage->EnemyShot.Data.Cnt[0] = x1 << 8;
	    manage->EnemyShot.Data.Cnt[1] = y1 << 8;
	    if (absx >= absy)
	    {
		manage->EnemyShot.Data.Cnt[2] = 1 << 8;
		manage->EnemyShot.Data.Cnt[3] = (absy/(double)absx) * 256;
	    }
	    else
	    {
		manage->EnemyShot.Data.Cnt[2] = (absx/(double)absy) * 256;
		manage->EnemyShot.Data.Cnt[3] = 1 << 8;
	    }
	    if (diffx < 0)
		manage->EnemyShot.Data.Cnt[2] *= -1;
	    if (diffy < 0)
		manage->EnemyShot.Data.Cnt[3] *= -1;

	    manage->enemy[i]->Data    = manage->EnemyShot.Data;
	    manage->enemy[i]->Grp     = manage->EnemyShot.Grp;
            manage->enemy[i]->Action  = EnemyShotAct;
            manage->enemy[i]->Realize = DrawImage;
            manage->enemy[i]->Hit     = NullDelHit;

	    manage->EnemyNum++;
	    return;
	}
    }
}

DelAtt EnemyShotAct(ObjData *my)
{
  /* 2^8 = 256 */
    my->Cnt[0] += my->Cnt[2]*my->Speed;
    my->X = my->Cnt[0] / 256;
    my->Cnt[1] += my->Cnt[3]*my->Speed;
    my->Y = my->Cnt[1] / 256;

    my->Cnt[4]++;
    if (my->Cnt[4] >= 3)
    {
	my->Cnt[4] = 0;
	my->image++;
	if (my->image > 3)
	    my->image = 0;
    }

    if ((my->X < 0 - my->Width/2) || (my->X > FieldW + my->Width/2))
	return NullDel;
    if ((my->Y < 0 - my->Height/2) || (my->Y > FieldH + my->Height/2))
	return NullDel;

    return NoneDel;
}

int RingToAngle(int x, int y, int angle, int speed)
{
  if (speed <= 0)
    speed = 1;
  
    manage->New.Data.hitAtt = MEnemy;
    manage->New.Data.hitMask = MPlayer | MPShot;

    manage->New.Data.X = x;
    manage->New.Data.Y = y;
    manage->New.Data.HP = 1;
    manage->New.Data.Point = 0;
    manage->New.Data.Angle = angle;
    manage->New.Data.Speed = speed;
    manage->New.Data.Cnt[0] = x << 8;
    manage->New.Data.Cnt[1] = y << 8;
    manage->New.Data.Cnt[2] = icos(angle);
    manage->New.Data.Cnt[3] = isin(angle);
    manage->New.Data.Cnt[4] = 0;
    manage->New.Data.EnemyAtt = NullDel;
    manage->New.Data.Width = 28;
    manage->New.Data.Height = 28;

    manage->New.Grp.image = ERingImage;

    return NewObj(MEnemy,EnemyShotAct,DeleteHit,DrawImage);
}

int RingToPoint(int x1, int y1, int x2, int y2, int speed)
{
    int diffx = x2 - x1;
    int diffy = y2 - y1;
    int absx = abs(diffx);
    int absy = abs(diffy);

    if (speed <= 0)
      speed = 1;
    
    manage->New.Data.hitAtt = MEnemy;
    manage->New.Data.hitMask = MPlayer | MPShot;

    manage->New.Data.X = x1;
    manage->New.Data.Y = y1;
    manage->New.Data.HP = 1;
    manage->New.Data.Point = 0;
    manage->New.Data.Speed = speed;
    manage->New.Data.Cnt[0] = x1 << 8;
    manage->New.Data.Cnt[1] = y1 << 8;

    if (absx >= absy)
    {
	manage->New.Data.Cnt[2] = 1 << 8;
	manage->New.Data.Cnt[3] = (absy/(double)absx) * 256;
    }
    else
    {
	manage->New.Data.Cnt[2] = (absx/(double)absy) * 256;
	manage->New.Data.Cnt[3] = 1 << 8;
    }
    if (diffx < 0)
	manage->New.Data.Cnt[2] *= -1;
    if (diffy < 0)
	manage->New.Data.Cnt[3] *= -1;

    manage->New.Data.Cnt[4] = 0;
    manage->New.Data.EnemyAtt = NullDel;
    manage->New.Data.Width = 28;
    manage->New.Data.Height = 28;

    manage->New.Grp.image = ERingImage;

    return NewObj(MEnemy,EnemyShotAct,DeleteHit,DrawImage);
}

int HomingShot(int x, int y, int ix, int iy)
{
    manage->New.Data.hitAtt = MEnemy;
    manage->New.Data.hitMask = MPlayer | MPShot;

    manage->New.Data.X = x;
    manage->New.Data.Y = y;
    manage->New.Data.HP = 1;
    manage->New.Data.Point = 0;
    manage->New.Data.EnemyAtt = NullDel;
    manage->New.Data.Width = 16;
    manage->New.Data.Height = 16;
    manage->New.Data.inertX = ix;
    manage->New.Data.inertY = iy;

    manage->New.Grp.image = EMissileImage;

    return NewObj(MEnemy,HomingAct,DeleteHit,DrawImage);
}

DelAtt HomingAct(ObjData *my)
{
    if (my->X < manage->player[0]->Data.X)
    {
	if (my->inertX < 15)
            my->inertX += 1;
    }
    else
    {
	if (my->inertX > -15)
            my->inertX -= 1;
    }    

    if (my->Y < manage->player[0]->Data.Y)
	my->inertY++;
    else
	my->inertY--;

    my->X += my->inertX;
    my->Y += my->inertY;

    my->image = GetDirection(0,0,my->inertX,my->inertY);

    if ((my->X < 0 - my->Width/2) || (my->X > FieldW + my->Width/2))
	return NullDel;
    if ((my->Y < 0 - my->Height/2) || (my->Y > FieldH + my->Height/2))
	return NullDel;

    return NoneDel;
}

int LaserShot(int x, int y, int speed)
{
  if (speed <= 0)
    speed = 1;
  
    manage->New.Data.hitAtt = MEShot;
    manage->New.Data.hitMask = MPlayer;

    manage->New.Data.X = x;
    manage->New.Data.Y = y;
    manage->New.Data.EnemyAtt = NullDel;
    manage->New.Data.Width = 7;
    manage->New.Data.Height = 50;
    manage->New.Data.Speed = speed;

    manage->New.Grp.image = ELaserImage;

    return NewObj(MEShot,EnemyLaserAct,NullHit,DrawImage);
}

DelAtt EnemyLaserAct(ObjData *my)
{
    my->Y += my->Speed;

    if ((my->X < 0 - my->Width/2) || (my->X > FieldW + my->Width/2))
	return NullDel;
    if ((my->Y < 0 - my->Height/2) || (my->Y > FieldH + my->Height/2))
	return NullDel;

    return NoneDel;
}

int BoundShot(int x, int y, int ix, int iy, int bound)
{
  if ((ix == 0) && (iy == 0))
    ix = 1;

    manage->New.Data.hitAtt = MEnemy;
    manage->New.Data.hitMask = MPlayer | MPShot;

    manage->New.Data.X = x;
    manage->New.Data.Y = y;
    manage->New.Data.EnemyAtt = NullDel;
    manage->New.Data.Width = 28;
    manage->New.Data.Height = 28;
    manage->New.Data.inertX = ix;
    manage->New.Data.inertY = iy;

    manage->New.Data.Cnt[0] = 0;
    manage->New.Data.Cnt[1] = bound;

    manage->New.Grp.image = EBoundImage;

    return NewObj(MEShot,BoundShotAct,NullHit,DrawImage);
}

DelAtt BoundShotAct(ObjData *my)
{
  if (my->Cnt[0] <= my->Cnt[1])
  {
    if ((my->X+my->inertX>FieldW) || (my->X+my->inertX<0))
    {
      my->inertX = my->inertX*(-1);
      my->Cnt[0]++;
    }
    if ((my->Y+my->inertY>FieldH) || (my->Y+my->inertY<0))
    {
      my->inertY = my->inertY*(-1);
      my->Cnt[0]++;
    }
  }
    
    my->image++;
    if (my->image >= 8)
	my->image = 0;

    my->X += my->inertX;
    my->Y += my->inertY;

    if ((my->X < 0 - my->Width/2) || (my->X > FieldW + my->Width/2))
	return NullDel;
    if ((my->Y < 0 - my->Height/2) || (my->Y > FieldH + my->Height/2))
	return NullDel;

    return NoneDel;
}
