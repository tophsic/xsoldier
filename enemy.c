/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: enemy.c,v 1.16 2002/04/29 03:40:19 oohara Exp $ */

/* rand */
#include <stdlib.h>
/* abs */
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
#include "enemy.h"
#include "extern.h"
#include "sin.h"

/* definition of objects
 * - initialization function
 * - action function
 * - hit function
 *
 * initialization add action function and display/hit function to the table
 */

/* run straight ahead */
int NewEnemy1(int x, int y)
{
  /* attribute of this object */
    manage->New.Data.hitAtt = MEnemy;
    /* what objects should hit this object */
    manage->New.Data.hitMask = MPlayer | MPShot;

    manage->New.Data.HP = 1;
    manage->New.Data.Point = 100;
    manage->New.Data.EnemyAtt = ZakoDel;

    /* size for collision detection */
    manage->New.Data.Width = 32;
    manage->New.Data.Height = 32;

    manage->New.Data.X = x;
    manage->New.Data.Y = 1 - manage->New.Data.Height/2;

    manage->New.Data.Angle = 0;
    manage->New.Data.Speed = rand()%15 + 1;

    manage->New.Data.Cnt[0] = 0;

    /* pixmap for this object */
    manage->New.Grp.image = Enemy1Image;

    /* add action function and hit function to the table */
    return NewObj(MEnemy,EnemyAct1,EnemyHit1,DrawImage);
}

DelAtt EnemyAct1(ObjData *my)
{
  int i;
    if (manage->Loop > 1)
    {
      /* shoot in loop 2 or later */
	if (my->shotTime >= ShotTiming)
	{
          for (i =1; i< manage->Loop; i++)
	    ShotToPoint(my->X,my->Y,manage->player[0]->Data.X,manage->player[0]->Data.Y,5);
	    my->shotTime = my->startTime;
	}
	else
	    my->shotTime++;
    }
    
    my->image = 4;
    my->Y += my->Speed;

    if ((my->X < 0 - my->Width/2) || (my->X > FieldW + my->Width/2))
	return NullDel;
    if ((my->Y < 0 - my->Height/2) || (my->Y > FieldH + my->Height/2))
	return NullDel;

    return NoneDel;
}

/* may give you an item */
DelAtt EnemyHit1(ObjData *my, ObjData *your)
{
  int temp = your->Attack;
    if (my->HP < your->Attack)
      temp = my->HP;

    my->HP -= temp;
    player->Rec[0].score += temp;

    if (my->HP <= 0)
    {
      player->Rec[0].score -= 1;

      /* always gives you a shot in loop 3 or later */
	if (manage->Loop > 2)
	    ShotToPoint(my->X,my->Y,manage->player[0]->Data.X,manage->player[0]->Data.Y,5);

	if (rand()%100 < 20)
	    NewEnemy10(my->X,my->Y);
        NewBomb(my->X,my->Y);

        my->showDamegeTime = 0;
        return my->EnemyAtt;
    }
    else
    {
      my->showDamegeTime = 15;
      return NoneDel;
    }
    
}

/* codename "sine curve" (in fact it is a parabola) */
int NewEnemy2(int x, int y)
{

    manage->New.Data.hitAtt = MEnemy;
    manage->New.Data.hitMask = MPlayer | MPShot;

    manage->New.Data.HP = 1;
    manage->New.Data.Point = 100;
    manage->New.Data.EnemyAtt = ZakoDel;

    manage->New.Data.Width = 32;
    manage->New.Data.Height = 32;

    manage->New.Data.X = x;
    manage->New.Data.Y = 1 - manage->New.Data.Height/2;

    manage->New.Data.inertX = 0;
    manage->New.Data.inertY = 1;

    manage->New.Data.Angle = 0;
    manage->New.Data.Speed = 0;
    manage->New.Data.Cnt[0] = 0;
    /* range of x */
    manage->New.Data.Cnt[1] = rand()%3 + 5;
    if (x < FieldW/2)
	manage->New.Data.Cnt[2] = 0;
    else
	manage->New.Data.Cnt[2] = 1;

    manage->New.Data.Cnt[3] = 0;

    manage->New.Grp.image = Enemy3Image;

    return NewObj(MEnemy,EnemyAct2,DamageHit,DrawImage);
}

DelAtt EnemyAct2(ObjData *my)
{
    if (my->shotTime >= ShotTiming)
    {
	if (manage->Loop > 1)
	    LaserShot(my->X,my->Y,10);
        ShotToPoint(my->X,my->Y,manage->player[0]->Data.X,manage->player[0]->Data.Y,4);
	my->shotTime = my->startTime;
    }
    else
	my->shotTime++;

    if (my->Cnt[2] == 0)
    {
        my->Cnt[3]++;

        if(my->Cnt[3] >= 2*my->Cnt[1])
            my->Cnt[2] = 1;
    }
    else if (my->Cnt[2] == 1)
    {
        my->Cnt[3]--;

        if(my->Cnt[3] <= -2*my->Cnt[1])
            my->Cnt[2] = 0;
    }

    my->Y += my->Cnt[1];
    my->X += my->Cnt[3];

    /* what number of pixmap we use */
    if (my->Cnt[3] > my->Cnt[1])
        my->image = 2;
    else if (my->Cnt[3] > 0)
        my->image = 3;
    else if (my->Cnt[3] < -my->Cnt[1])
        my->image = 6;
    else if(my->Cnt[3] < 0)
        my->image = 5;
    else
        my->image = 0;

    if ((my->Y < 0 - my->Height/2) || (my->Y > FieldH + my->Height/2))
	return NullDel;

    return NoneDel;
}

/* turns left/right */
int NewEnemy3(int x, int y)
{

    manage->New.Data.hitAtt = MEnemy;
    manage->New.Data.hitMask = MPlayer | MPShot;

    manage->New.Data.HP = 2;
    manage->New.Data.Point = 150;
    manage->New.Data.EnemyAtt = ZakoDel;

    manage->New.Data.Width = 32;
    manage->New.Data.Height = 32;

    manage->New.Data.X = x;
    manage->New.Data.Y = 1 - manage->New.Data.Height/2;

    manage->New.Data.Cnt[0] = 0;
    manage->New.Data.Cnt[1] = 0;
    manage->New.Data.Cnt[2] = 5;

    manage->New.Grp.image = Enemy1Image;
    return NewObj(MEnemy,EnemyAct3,EnemyHit1,DrawImage);
}

DelAtt EnemyAct3(ObjData *my)
{
    if (my->shotTime >= ShotTiming)
    {
        ShotToPoint(my->X,my->Y,manage->player[0]->Data.X,manage->player[0]->Data.Y,4);
        my->shotTime = my->startTime;
    }
    else
        my->shotTime++;
    
    if (my->Cnt[0] == 0)
    {
	if (my->Y<manage->player[0]->Data.Y+10 && my->Y>manage->player[0]->Data.Y-10)
	{
	    if (my->X < manage->player[0]->Data.X)
            {
              my->Cnt[0] = 1;
              if (manage->Loop >2)
                BoundShot(my->X, my->Y, rand()%30-15, rand()%30-15, 3);
            }
            
	    if (my->X > manage->player[0]->Data.X)
            {
              my->Cnt[0] = 2;
              if (manage->Loop >2)
                BoundShot(my->X, my->Y, rand()%30-15, rand()%30-15, 3);
            }
	}
    }

    if (my->Cnt[0] == 0)
    {
	my->Y += 2 + manage->Loop * 3;
	my->image = 4;
    }
    else if (my->Cnt[0] == 1)
    {
	my->X += 15;
	my->image = 2;
    }
    else if (my->Cnt[0] == 2)
    {
	my->X -= 15;
	my->image = 6;
    }

    if ((my->X < 0 - my->Width/2) || (my->X > FieldW + my->Width/2))
	return NullDel;
    if ((my->Y < 0 - my->Height/2) || (my->Y > FieldH + my->Height/2))
	return NullDel;

    return NoneDel;
}

/* chases you */
int NewEnemy4(int x, int y)
{

    manage->New.Data.hitAtt = MEnemy;
    manage->New.Data.hitMask = MPlayer | MPShot;

    manage->New.Data.HP = 2;
    manage->New.Data.Point = 150;
    manage->New.Data.EnemyAtt = ZakoDel;

    manage->New.Data.Width = 32;
    manage->New.Data.Height = 32;

    manage->New.Data.X = x;
    manage->New.Data.Y = 1 - manage->New.Data.Height/2;

    manage->New.Data.inertX = 0;
    manage->New.Data.inertY = 0;

    manage->New.Grp.image = Enemy2Image;
    return NewObj(MEnemy,EnemyAct4,DamageHit,DrawImage);
}

DelAtt EnemyAct4(ObjData *my)
{
    if (my->shotTime >= ShotTiming)
    {
        ShotToPoint(my->X,my->Y,manage->player[0]->Data.X,manage->player[0]->Data.Y,4);
        if (manage->Loop >1)
        {
          ShotToAngle(my->X, my->Y, 90, 4);
          ShotToAngle(my->X, my->Y, 90, 8);
          ShotToAngle(my->X, my->Y, 90, 12);
          ShotToAngle(my->X, my->Y, 90, 16);
        }
        
	my->shotTime = my->startTime;
    }
    else
	my->shotTime++;

    if (my->X < manage->player[0]->Data.X)
    {
      if (my->inertX <= 8)
        my->inertX++;
    }
    else if (my->X > manage->player[0]->Data.X)
    {
      if (my->inertX >= -8)
        my->inertX--;
    }

    if (my->Y < manage->player[0]->Data.Y)
    {
      if (my->inertY <= 8)
        my->inertY++;
    }
    else if (my->Y > manage->player[0]->Data.Y)
    {
      if (my->inertY >= -8)
        my->inertY--;
    }
    
    my->X += my->inertX;
    my->Y += my->inertY;

    my->image = GetDirection(my->X,my->Y,manage->player[0]->Data.X,manage->player[0]->Data.Y);
    
    if ((my->X < 0 - my->Width/2) || (my->X > FieldW + my->Width/2))
	return NullDel;
    if ((my->Y < 0 - my->Height/2) || (my->Y > FieldH + my->Height/2))
	return NullDel;

    return NoneDel;
}

/* bounce on the left/right side of the window */
int NewEnemy5(int x, int y)
{

    manage->New.Data.hitAtt = MEnemy;
    manage->New.Data.hitMask = MPlayer | MPShot;

    if (x > FieldW/2)
    {
	manage->New.Data.X = FieldW;
	manage->New.Data.Cnt[0] = - (rand()%10 + 15);
    }
    else
    {
	manage->New.Data.X = 0;
	manage->New.Data.Cnt[0] = rand()%10 + 15;
    }

    manage->New.Data.HP = 9;
    manage->New.Data.Point = 200;
    manage->New.Data.EnemyAtt = ZakoDel;

    manage->New.Data.Width = 32;
    manage->New.Data.Height = 32;

    manage->New.Data.Y = 1 - manage->New.Data.Height/2;

    manage->New.Data.inertX = 0;
    manage->New.Data.inertY = 0;

    manage->New.Grp.image = Enemy5Image;
    return NewObj(MEnemy,EnemyAct5,EnemyHit1,DrawImage);
}

DelAtt EnemyAct5(ObjData *my)
{
    if (my->Cnt[0] < 0)
    {
	my->inertX++;
	if (abs(my->inertX+my->Cnt[0]) < 5)
	    my->image = 3;
	else
	    my->image = 2;
    }
    else
    {
	my->inertX--;
	if (abs(my->inertX+my->Cnt[0]) < 5)
	    my->image = 1;
	else
	    my->image = 0;
    }

    if (my->Cnt[0]+my->inertX == 0)
    {
	RingToPoint(my->X,my->Y,manage->player[0]->Data.X,manage->player[0]->Data.Y,rand()%4+2);
	RingToAngle(my->X, my->Y, 45, 5);
	RingToAngle(my->X, my->Y, 90, 5);
	RingToAngle(my->X, my->Y, 135, 5);
	RingToPoint(my->X,my->Y,manage->player[0]->Data.X,manage->player[0]->Data.Y,rand()%2+4);
	if (manage->Loop > 1)
	{
	    RingToPoint(my->X,my->Y,manage->player[0]->Data.X,manage->player[0]->Data.Y,rand()%3+3);
	    RingToPoint(my->X,my->Y,manage->player[0]->Data.X,manage->player[0]->Data.Y,rand()%4+4);
	}
    }

    my->X += (my->Cnt[0] + my->inertX);
    my->Y += 5;

    if ((my->X < 0) || (my->X > FieldW))
	my->inertX = 0;

    if ((my->Y < 0 - my->Height/2) || (my->Y > FieldH + my->Height/2))
	return NullDel;

    return NoneDel;
}

/* bounce on any side of the window */
int NewEnemy6(int x, int y)
{

    manage->New.Data.hitAtt = MEnemy;
    manage->New.Data.hitMask = MPlayer | MPShot;

    manage->New.Data.HP = 4;
    manage->New.Data.Point = 200;
    manage->New.Data.EnemyAtt = ZakoDel;

    manage->New.Data.Width = 32;
    manage->New.Data.Height = 32;

    manage->New.Data.X = x;
    manage->New.Data.Y = 1 - manage->New.Data.Height/2;

    if (x > FieldW/2)
	manage->New.Data.inertX = rand()%5 + 10;
    else
	manage->New.Data.inertX = rand()%5 - 15;

    manage->New.Data.inertY = rand()%10 + 5;

    manage->New.Data.Cnt[0] = 0;
    manage->New.Data.Cnt[1] = 8;
    /* "appeared" counter */
    manage->New.Data.Cnt[2] = 0;

    manage->New.Grp.image = Enemy4Image;
    return NewObj(MEnemy,EnemyAct6,DamageHit,DrawImage);
}

DelAtt EnemyAct6(ObjData *my)
{
    if (my->shotTime >= ShotTiming)
    {
	ShotToPoint(my->X,my->Y,manage->player[0]->Data.X,manage->player[0]->Data.Y,rand()%4+ manage->Loop * 7 -5);
	my->shotTime = my->startTime;
    }
    else
	my->shotTime++;

    if ((my->Cnt[2] != 0) && (my->Cnt[0] <= my->Cnt[1]))
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
    if (my->image > 7)
	my->image = 0;

    my->X += my->inertX;
    my->Y += my->inertY;

    if ((my->X < 0 - my->Width/2) || (my->X > FieldW + my->Width/2))
	return NullDel;
    if ((my->Y < 0 - my->Height/2) || (my->Y > FieldH + my->Height/2))
	return NullDel;

    if ((my->Cnt[2] == 0) && (my->X >= 0) && (my->X <= FieldW)
        && (my->Y >= 0) && (my->Y < FieldH))
      my->Cnt[2] = 1;
    
    return NoneDel;
}

/* appears from the lower side of the window */
int NewEnemy7(int x, int y)
{

    manage->New.Data.hitAtt = MEnemy;
    manage->New.Data.hitMask = MPlayer | MPShot;

    if (x > FieldW/2)
	manage->New.Data.X = FieldW -50;
    else
	manage->New.Data.X =         50;

    manage->New.Data.HP = 3;
    manage->New.Data.Point = 250;
    manage->New.Data.EnemyAtt = ZakoDel;

    manage->New.Data.Width = 32;
    manage->New.Data.Height = 32;

    manage->New.Data.Y = FieldH - 1 + manage->New.Data.Height/2;

    manage->New.Data.Cnt[1] = 0;

    manage->New.Grp.image = Enemy3Image;

    return NewObj(MEnemy,EnemyAct7,EnemyHit1,DrawImage);
}

DelAtt EnemyAct7(ObjData *my)
{
    if (manage->Loop > 1)
    {
	if (my->shotTime >= ShotTiming)
	{
	    HomingShot(my->X,my->Y,0,0);
	    my->shotTime = my->startTime;
	}
	else
	    my->shotTime++;
    }

    if (my->Cnt[1] > 20)
    {
	my->Y -= 9;
	my->image = 0;
    }
    else if (my->Y > FieldH-100)
    {
	my->Y -= 6;
	my->image = 0;
    }
    else
    {
	my->Cnt[1]++;
	my->image++;
	if (my->image > 7)
	    my->image = 0;
    }

    if (my->Cnt[1] == 20)
    {
        ShotToAngle(my->X, my->Y, 144, 10);
        ShotToAngle(my->X, my->Y, 180, 10);
        ShotToAngle(my->X, my->Y, 216, 10);
        ShotToAngle(my->X, my->Y, 252, 10);
        ShotToAngle(my->X, my->Y, 288, 10);
        ShotToAngle(my->X, my->Y, 324, 10);
        ShotToAngle(my->X, my->Y, 360, 10);
        ShotToAngle(my->X, my->Y, 36, 10);
    }

    if ((my->X < 0 - my->Width/2) || (my->X > FieldW + my->Width/2))
	return NullDel;
    if ((my->Y < 0 - my->Height/2) || (my->Y > FieldH + my->Height/2))
	return NullDel;

    return NoneDel;
}

/* run straight ahead and shoot homing missiles */
int NewEnemy8(int x, int y)
{

    manage->New.Data.hitAtt = MEnemy;
    manage->New.Data.hitMask = MPlayer | MPShot;

    manage->New.Data.HP = 15;
    manage->New.Data.Point = 500;
    manage->New.Data.EnemyAtt = ZakoDel;

    manage->New.Data.Width = 40;
    manage->New.Data.Height = 50;

    manage->New.Data.X = x;
    manage->New.Data.Y = 1 - manage->New.Data.Height/2;

    manage->New.Data.Angle = 0;
    manage->New.Data.Speed = rand()%5 + 1;

    manage->New.Grp.image = Enemy7Image;

    return NewObj(MEnemy,EnemyAct8,LargeDamageHit,DrawImage);
}

DelAtt EnemyAct8(ObjData *my)
{
    if (my->shotTime >= ShotTiming)
    {
        HomingShot(my->X+10,my->Y-20,-10,-5);
        HomingShot(my->X-10,my->Y-20,10,-5);

	if (manage->Loop > 1)
	{
	    LaserShot(my->X-15,my->Y-20,10);
	    LaserShot(my->X+15,my->Y-20,10);
	}
	my->shotTime = my->startTime;
    }
    else
	my->shotTime++;
    
    my->Y += my->Speed;
    
    if ((my->X < 0 - my->Width/2) || (my->X > FieldW + my->Width/2))
	return NullDel;
    if ((my->Y < 0 - my->Height/2) || (my->Y > FieldH + my->Height/2))
	return NullDel;

    return NoneDel;
}

/* "reversed" parabola */
int NewEnemy9(int x, int y)
{
    manage->New.Data.hitAtt = MEnemy;
    manage->New.Data.hitMask = MPlayer | MPShot;

    manage->New.Data.HP = 6;
    manage->New.Data.Point = 300;
    manage->New.Data.EnemyAtt = ZakoDel;

    manage->New.Data.Width = 32;
    manage->New.Data.Height = 32;

    manage->New.Data.X = x;
    manage->New.Data.Y = 1 - manage->New.Data.Height/2;

    if (x > FieldW/2)
	manage->New.Data.inertX = -(rand()%10+1);
    else
	manage->New.Data.inertX = rand()%10+1;
    manage->New.Data.inertY = rand()%10 + 15;

    manage->New.Data.Cnt[0] = 0;

    manage->New.Grp.image = Enemy6Image;
    return NewObj(MEnemy,EnemyAct9,DamageHit,DrawImage);
}

DelAtt EnemyAct9(ObjData *my)
{
    if (manage->Loop > 2)
    {
	if (my->shotTime >= ShotTiming)
	{
	    ShotToAngle(my->X, my->Y, 50, 25);
	    ShotToAngle(my->X, my->Y, 70, 25);
	    ShotToAngle(my->X, my->Y, 90, 25);
	    ShotToAngle(my->X, my->Y, 110, 25);
	    ShotToAngle(my->X, my->Y, 130, 25);
	    my->shotTime = my->startTime;
	}
	else
	    my->shotTime++;
    }

    if (my->inertY<0 && my->inertY%2)
        ShotToPoint(my->X,my->Y,manage->player[0]->Data.X,manage->player[0]->Data.Y,rand()%3+3);
	
    my->inertY--;

    my->X += my->inertX;
    my->Y += my->inertY;

    if (my->inertY > 10)
	my->image = 0;
    else if (my->inertY > 5)
	my->image = 1;
    else if (my->inertY > 0)
	my->image = 2;
    else if (my->inertY > -5)
	my->image = 3;
    else if (my->inertY > -10)
	my->image = 4;
    else
	my->image = 5;

    if ((my->X < 0 - my->Width/2) || (my->X > FieldW + my->Width/2))
	return NullDel;
    if ((my->Y < 0 - my->Height/2) || (my->Y > FieldH + my->Height/2))
	return NullDel;

    return NoneDel;
}

/* power-up item */
int NewEnemy10(int x, int y)
{
    manage->New.Data.hitAtt = MItem;
    manage->New.Data.hitMask = MPlayer;

    manage->New.Data.X = x;
    manage->New.Data.Y = y;

    manage->New.Data.HP = 0;
    manage->New.Data.Point = 0;
    manage->New.Data.EnemyAtt = NoneDel;

    manage->New.Data.Width = 32;
    manage->New.Data.Height = 32;

    manage->New.Data.inertY = -10 - rand() % 10;

    /* item counter */
    manage->New.Data.Cnt[0] = 0;
    /* time counter */
    manage->New.Data.Cnt[1] = 0;
    /* max speed */
    manage->New.Data.Cnt[2] = rand()%6 + 1;
    if (rand()%100 < 30)
    {
	manage->New.Data.Cnt[3] = 1;
	manage->New.Data.Cnt[0] = 1;
    }
    else
	manage->New.Data.Cnt[3] = 0;

    manage->New.Grp.image = ItemImage;
    return NewObj(MEnemy,EnemyAct10,NullDelHit,DrawImage);
}

DelAtt EnemyAct10(ObjData *my)
{
    my->Cnt[1]++;

    if (my->inertY < my->Cnt[2])
	my->inertY++;
    my->Y += my->inertY;

    if (my->Cnt[3] == 1)
    {
	if (my->Cnt[1]%25 == 0)
	{
	    if (my->Cnt[0] < 3)
		my->Cnt[0]++;
	    else
		my->Cnt[0] = 1;
	}
    }

    my->image = my->Cnt[0];

    if ((my->X < 0 - my->Width/2) || (my->X > FieldW + my->Width/2))
	return NullDel;
    if (my->Y > FieldH + my->Height/2)
	return NullDel;

    return NoneDel;
}
