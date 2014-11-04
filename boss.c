/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: boss.c,v 1.21 2002/05/26 08:31:59 oohara Exp $ */

/* DEBUG is defined in config.h */
#include <config.h>
/* rand */
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
#include "boss.h"
#include "enemy.h"
#include "extern.h"
#include "sin.h"

/* local functions for the last boss */
static DelAtt BossAct8_dead(ObjData *my);
static DelAtt BossAct8_rand_shot(ObjData *my);
static DelAtt BossAct8_ring_shot(ObjData *my);
static DelAtt BossAct8_bound_shot(ObjData *my);
static DelAtt BossAct8_homing_shot(ObjData *my);
static DelAtt BossAct8_charge_shot(ObjData *my);
static DelAtt BossAct8_n_way_shot(ObjData *my);
static void BossAct8_next(ObjData *my, int span);

/* definition of objects
 * - initialization function
 * - action function
 * - hit function
 *
 * initialization add action function and display/hit function to the table
 */

int NewBoss1(void)
{
    manage->New.Data.hitAtt = MEnemy;
    manage->New.Data.hitMask = MPlayer | MPShot;

    manage->New.Data.X = FieldW/2;
    manage->New.Data.Y = -90;

    manage->New.Data.HP = 120;
    manage->New.Data.Point = 5000;
    manage->New.Data.EnemyAtt = BossDel;

    manage->New.Data.Width = 150;
    manage->New.Data.Height = 180;

    manage->New.Data.Cnt[0] = 0;
    manage->New.Data.Cnt[1] = 200; /* immutable time */
    manage->New.Data.Cnt[2] = 0; /* image counter*/
    manage->New.Data.Cnt[3] = 10; /* x move */
    manage->New.Data.Cnt[4] = 0; /* y move */
    manage->New.Data.Cnt[5] = 0;  /* laser counter */
    manage->New.Data.Cnt[6] = 0;  /* normal shot counter */

    manage->New.Grp.image = Boss1Image;
    return NewObj(MEnemy,BossAct1,BossHit1,DrawImage);
}

DelAtt BossAct1(ObjData *my)
{
  int i;
  
    if (my->Cnt[0] < my->Cnt[1])
    {/* appears slowly */
	my->Y++;
	my->Cnt[0]++;
	return NoneDel;
    }

    if (manage->BossTime <= 0)
    {
      if (my->Y > 0 - my->Height/2 - 100)
        (my->Y) -= 4;
      if (my->Y < 0 - my->Height/2 - 50)
      {
        if ((player->Ships <= 0) && (manage->player[0]->Data.Cnt[3] != 0))
          return NoneDel;
        else
          return BossDel;
      }
      else
	return NoneDel;
    }

    (manage->BossTime)--;

    if (my->Cnt[6] >= (100- (manage->Loop)*10) )
    {
        ShotToAngle(my->X, my->Y, 50, 10);
        ShotToAngle(my->X, my->Y, 70, 10);
        ShotToAngle(my->X, my->Y, 90, 10);
        ShotToAngle(my->X, my->Y, 110, 10);
        ShotToAngle(my->X, my->Y, 130, 10);
        my->Cnt[6] =0;
    }
    else
	(my->Cnt[6])++;

    if (my->Cnt[5] >= (25-manage->Loop*3))
    {
      for (i = 1; i< manage->Loop; i++)
	    ShotToAngle(my->X,my->Y,rand()%180,10);
      if (manage->Loop >1)
	    ShotToPoint(my->X,my->Y,manage->player[0]->Data.X,manage->player[0]->Data.Y,10);
	LaserShot(my->X-30,my->Y,20);
	LaserShot(my->X+30,my->Y,20);
	LaserShot(my->X-80,my->Y,20);
	LaserShot(my->X+80,my->Y,20);

	my->Cnt[5] = 0;
    }
    else
	my->Cnt[5]++;

    if ((my->X+my->Cnt[3]>FieldW) || (my->X+my->Cnt[3]<0))
	my->Cnt[3] = my->Cnt[3]*(-1);

    my->X += my->Cnt[3];

    return NoneDel;
}

DelAtt BossHit1(ObjData *my, ObjData *your)
{
  int temp = your->Attack;
  
  if ((my->Cnt[0] < my->Cnt[1]) || (manage->BossTime <=0))
    {
      return NoneDel;
    }
    
    if (my->HP < temp)
    {
      temp = my->HP;
    }
    
    my->HP -= temp;
    player->Rec[0].score += temp;
    
    if (my->HP <= 0)
    {
      player->Rec[0].score -= 1;
        NewBomb(my->X,my->Y);
        NewBomb(my->X+rand()%100-50,my->Y+rand()%100-50);
        NewBomb(my->X+rand()%100-50,my->Y+rand()%100-50);
        NewBomb(my->X+rand()%100-50,my->Y+rand()%100-50);
        NewBomb(my->X+rand()%100-50,my->Y+rand()%100-50);
        NewBomb(my->X+rand()%100-50,my->Y+rand()%100-50);
        NewBomb(my->X+rand()%100-50,my->Y+rand()%100-50);

        my->showDamegeTime = 0;
        return my->EnemyAtt;
    }
    else
    {
      my->showDamegeTime = 15;
      return NoneDel;
    }
    
}

int NewBoss2(void)
{
    manage->New.Data.hitAtt = MEnemy;
    manage->New.Data.hitMask = MPlayer | MPShot;

    manage->New.Data.X = FieldW/2;
    manage->New.Data.Y = -60;

    manage->New.Data.HP = 200;
    manage->New.Data.Point = 5000;
    manage->New.Data.EnemyAtt = BossDel;

    manage->New.Data.Width = 90;
    manage->New.Data.Height = 130;

    manage->New.Data.Cnt[0] = 0;
    manage->New.Data.Cnt[1] = 200; /* immutable time */
    manage->New.Data.Cnt[2] = 0; /* image counter */

    manage->New.Data.Cnt[3] = 0;
    manage->New.Data.Cnt[4] = 0;
    manage->New.Data.Cnt[5] = 0; /* normal shot counter */
    
    manage->New.Grp.image = Boss2Image;
    return NewObj(MEnemy,BossAct2,BossHit1,DrawImage);
}

DelAtt BossAct2(ObjData *my)
{
    if (my->Cnt[0] < my->Cnt[1])
    {/* appears slowly */
	my->Y++;
	my->Cnt[0]++;
	return NoneDel;
    }

    if (manage->BossTime <= 0)
    {
      if (my->Y > 0 - my->Height/2 - 100)
        (my->Y) -= 4;
      if (my->Y < 0 - my->Height/2 - 50)
      {
        if ((player->Ships <= 0) && (manage->player[0]->Data.Cnt[3] != 0))
          return NoneDel;
        else
          return BossDel;
      }
      else
	return NoneDel;
    }

    (manage->BossTime)--;

    if (manage->Loop > 2)
    {
	if (my->Cnt[5] >= 90- (manage->Loop)*10)
	{
	    ShotToPoint(my->X+40,my->Y,manage->player[0]->Data.X,manage->player[0]->Data.Y,rand()%3 + 5);
	    ShotToPoint(my->X   ,my->Y,manage->player[0]->Data.X,manage->player[0]->Data.Y,8);
	    ShotToPoint(my->X-40,my->Y,manage->player[0]->Data.X,manage->player[0]->Data.Y,rand()%5 + 3);
            my->Cnt[5] = 0;
	}
	else
            (my->Cnt[5])++;
    }

    if (my->Cnt[3] >= (15-manage->Loop*3))
    {
      if (manage->Loop > 1)
      {
        if (manage->player[0]->Data.Y < my->Y)
          ShotToAngle(my->X, my->Y, 270, 10);
        else
          ShotToAngle(my->X, my->Y, 90, 10);
      }
      
      HomingShot(my->X-20,my->Y-10,-10,-10);
      HomingShot(my->X+20,my->Y-10, 10,-10);
      my->Cnt[3] = 0;
    }
    else
	my->Cnt[3]++;

    if (my->X > manage->player[0]->Data.X)
    {
	if (my->X - manage->player[0]->Data.X > 20)
	{
	    my->X -= 10;
	}
	else
	{
	    my->Cnt[4]++;
	    my->X --;
	}

	if (my->Cnt[4] > 10)
	{
	    LaserShot(my->X-40,my->Y,30);
	    LaserShot(my->X+40,my->Y,30);
	    my->Cnt[4] = 0;
	}
    }
    else
    {
	if (manage->player[0]->Data.X - my->X > 20)
	{
	    my->X += 10;
	}
	else
	{
	    my->Cnt[4]++;
	    my->X ++;
	}

	if (my->Cnt[4] > 10)
	{
	    LaserShot(my->X-40,my->Y,30);
	    LaserShot(my->X+40,my->Y,30);
	    my->Cnt[4] = 0;
	}
    }

    return NoneDel;
}

int NewBoss3(void)
{
    manage->New.Data.hitAtt = MEnemy;
    manage->New.Data.hitMask = MPlayer | MPShot;

    manage->New.Data.X = FieldW/2;
    manage->New.Data.Y = -80;

    manage->New.Data.HP = 330;
    manage->New.Data.Point = 6000;
    manage->New.Data.EnemyAtt = BossDel;

    manage->New.Data.Width = 130;
    manage->New.Data.Height = 130;

    manage->New.Data.Cnt[0] = 0;
    manage->New.Data.Cnt[1] = 200; /* immutable time */
    manage->New.Data.Cnt[2] = 0; /* image counter */
    manage->New.Data.Cnt[3] = 10; /* x move */
    manage->New.Data.Cnt[4] = 0; /* y move */
    manage->New.Data.Cnt[5] = 0;  /* laser counter */
    manage->New.Data.Cnt[6] = 0; /* missile counter */

    manage->New.Grp.image = Boss3Image;
    return NewObj(MEnemy,BossAct3,BossHit1,DrawImage);
}

DelAtt BossAct3(ObjData *my)
{
    if (my->Cnt[0] < my->Cnt[1])
    {/* appears slowly */
	my->Y++;
	my->Cnt[0]++;
	return NoneDel;
    }

    if (manage->BossTime <= 0)
    {
      if (my->Y > 0 - my->Height/2 - 100)
        (my->Y) -= 4;
      if (my->Y < 0 - my->Height/2 - 50)
      {
        if ((player->Ships <= 0) && (manage->player[0]->Data.Cnt[3] != 0))
          return NoneDel;
        else
          return BossDel;
      }
      else
	return NoneDel;
    }

    (manage->BossTime)--;

    if (my->Cnt[6] >= (80 - manage->Loop*10))
    {
	HomingShot(my->X,my->Y,0,10);
	HomingShot(my->X,my->Y,10,0);
	HomingShot(my->X,my->Y,0,-10);
	HomingShot(my->X,my->Y,-10,0);
	HomingShot(my->X,my->Y,10,10);
	HomingShot(my->X,my->Y,-10,10);
	HomingShot(my->X,my->Y,10,-10);
	HomingShot(my->X,my->Y,-10,-10);

	my->Cnt[6] = 0;
    }
    else
      my->Cnt[6]++;

    if (my->Cnt[5] >= (25 - manage->Loop*2))
    {
      if (manage->Loop >1)
	LaserShot(my->X- 120+ (rand()%5)*60,my->Y,25);
      if (manage->Loop >2)
	    ShotToPoint(my->X,my->Y,manage->player[0]->Data.X,manage->player[0]->Data.Y,18);
	LaserShot(my->X-30,my->Y,25);
	LaserShot(my->X+30,my->Y,25);
	LaserShot(my->X-90,my->Y,25);
	LaserShot(my->X+90,my->Y,25);
	LaserShot(my->X-150,my->Y,25);
	LaserShot(my->X+150,my->Y,25);

	my->Cnt[5] = 0;
    }
    else
	my->Cnt[5]++;

    if ((my->X+my->Cnt[3]>FieldW) || (my->X+my->Cnt[3]<0))
	my->Cnt[3] = my->Cnt[3]*(-1);

    my->X += my->Cnt[3];

    return NoneDel;
}

int NewBoss4(void)
{
    manage->New.Data.hitAtt = MEnemy;
    manage->New.Data.hitMask = MPlayer | MPShot;

    manage->New.Data.X = FieldW/2;
    manage->New.Data.Y = -90;

    manage->New.Data.inertX = 0;
    manage->New.Data.inertY = 0;

    manage->New.Data.HP = 550;
    manage->New.Data.Point = 6000;
    manage->New.Data.EnemyAtt = BossDel;

    manage->New.Data.Width = 300;
    manage->New.Data.Height = 170;

    manage->New.Data.Cnt[0] = 0;
    manage->New.Data.Cnt[1] = 195; /* immutable time */
    manage->New.Data.Cnt[2] = 0; /* image counter */
    manage->New.Data.Cnt[3] = 1; /* x move */
    manage->New.Data.Cnt[4] = 0; /* y move */
    manage->New.Data.Cnt[5] = 0;  /* laser counter */

    manage->New.Grp.image = Boss4Image;
    return NewObj(MEnemy,BossAct4,BossHit1,DrawImage);
}

DelAtt BossAct4(ObjData *my)
{
    if (my->Cnt[0] < my->Cnt[1])
    {/* appears slowly */
	my->Y++;
	my->Cnt[0]++;
	return NoneDel;
    }

    if (manage->BossTime <= 0)
    {
      if (my->Y > 0 - my->Height/2 - 100)
        (my->Y) -= 4;
      if (my->Y < 0 - my->Height/2 - 50)
      {
        if ((player->Ships <= 0) && (manage->player[0]->Data.Cnt[3] != 0))
          return NoneDel;
        else
          return BossDel;
      }
      else
	return NoneDel;
    }

    (manage->BossTime)--;

    if (my->Cnt[5] >= (8-manage->Loop))
    {
	LaserShot(my->X + rand()%600-300, my->Y, 11 + rand()%3  + manage->Loop*7);
	LaserShot(my->X + rand()%600-300, my->Y, 6 + rand()%3 + manage->Loop*7);
	LaserShot(my->X + rand()%600-300, my->Y, 1 + rand()%3 + manage->Loop*7);
	LaserShot(my->X + rand()%600-300, my->Y, -4 + rand()%3 + manage->Loop*7);
        if (manage->Loop >2)
	LaserShot(my->X + rand()%600-300, my->Y, 6 + rand()%3 + manage->Loop*7);
        if (manage->Loop >1)
	LaserShot(my->X + rand()%600-300, my->Y, 1 + rand()%3 + manage->Loop*7);
	my->Cnt[5] = 0;
    }
    else
	my->Cnt[5]++;

    if (my->X < manage->player[0]->Data.X)
    {
        if (my->inertX <= 8)
            my->inertX++;
    }
    if (my->X > manage->player[0]->Data.X)
    {
        if (my->inertX >= -8)
            my->inertX--;
    }

    my->X += my->inertX;

    return NoneDel;
}

int NewBoss5(void)
{
    manage->New.Data.hitAtt = MEnemy;
    manage->New.Data.hitMask = MPlayer | MPShot;

    manage->New.Data.X = FieldW/2;
    manage->New.Data.Y = -128;

    manage->New.Data.HP = 350;
    manage->New.Data.Point = 7000;
    manage->New.Data.EnemyAtt = BossDel;

    manage->New.Data.Width = 120;
    manage->New.Data.Height = 120;

    manage->New.Data.Cnt[0] = 0;
    manage->New.Data.Cnt[1] = 200; /* immutable time */
    manage->New.Data.Cnt[2] = 0; /* image counter */
    manage->New.Data.Cnt[3] = manage->player[0]->Data.X; /* x move */
    manage->New.Data.Cnt[4] = manage->player[0]->Data.Y; /* y move */
    manage->New.Data.Cnt[5] = 0;  /* shot counter */
    manage->New.Data.Cnt[6] = 0;  /* angle counter */

    manage->New.Grp.image = Boss5Image;
    return NewObj(MEnemy,BossAct5,BossHit1,DrawImage);
}

DelAtt BossAct5(ObjData *my)
{
  int i;

    if (my->Cnt[0] < my->Cnt[1])
    {/* appears slowly */
	my->Y++;
	my->Cnt[0]++;
	return NoneDel;
    }

    if (manage->BossTime <= 0)
    {
      if (my->Y > 0 - my->Height/2 - 100)
        (my->Y) -= 4;
      if (my->Y < 0 - my->Height/2 - 50)
      {
        if ((player->Ships <= 0) && (manage->player[0]->Data.Cnt[3] != 0))
          return NoneDel;
        else
          return BossDel;
      }
      else
	return NoneDel;
    }

    (manage->BossTime)--;

    if (my->Cnt[5] > 120)
    {
	my->Cnt[3] = manage->player[0]->Data.X;
	my->Cnt[4] = manage->player[0]->Data.Y;
	my->Cnt[5] = 0;
    }
    else if (abs(my->X-my->Cnt[3]) > 5 || abs(my->Y-my->Cnt[4]) > 5)
    {
	if (my->X < my->Cnt[3])
	{
	    if (my->Cnt[3]-my->X > 50)
		my->X += 20;
	    else if (my->Cnt[3]-my->X > 30)
		my->X += 10;
	    else if (my->Cnt[3]-my->X > 20)
		my->X += 5;
	    else
		my->X ++;
	}
	else
	{
	    if (my->X-my->Cnt[3] > 50)
		my->X -= 20;
	    else if (my->X-my->Cnt[3] > 30)
		my->X -= 10;
	    else if (my->X-my->Cnt[3] > 20)
		my->X -= 5;
	    else
		my->X --;
	}

	if (my->Y < my->Cnt[4])
	{
	    if (my->Cnt[4]-my->Y > 50)
		my->Y += 20;
	    else if (my->Cnt[4]-my->Y > 30)
		my->Y += 10;
	    else if (my->Cnt[4]-my->Y > 20)
		my->Y += 5;
	    else
		my->Y ++;
	}
	else
	{
	    if (my->Y-my->Cnt[4] > 50)
		my->Y -= 20;
	    else if (my->Y-my->Cnt[4] > 30)
		my->Y -= 10;
	    else if (my->Y-my->Cnt[4] > 20)
		my->Y -= 5;
	    else
		my->Y --;
	}
    }
    else
    {
      if (manage->Loop >1)
      {
        if (my->Cnt[5] % 2 == 0)
        {
          for (i =1; i <manage->Loop; i++)
            ShotToAngle(my->X, my->Y, rand()%360, 3);
        }
      }

      my->Cnt[6] += 13;
      if (my->Cnt[6] >= 360)
        my->Cnt[6] -= 360;
      ShotToAngle(my->X,my->Y,my->Cnt[6],4 + manage->Loop * 4);

      my->Cnt[5]++;
    }

    return NoneDel;
}

int NewBoss6(void)
{
    manage->New.Data.hitAtt = MEnemy;
    manage->New.Data.hitMask = MPlayer | MPShot;

    manage->New.Data.X = FieldW/2;
    manage->New.Data.Y = -64;

    manage->New.Data.HP = 400;
    manage->New.Data.Point = 9000;
    manage->New.Data.EnemyAtt = BossDel;

    manage->New.Data.Width = 300;
    manage->New.Data.Height = 100;

    manage->New.Data.Cnt[0] = 0;
    manage->New.Data.Cnt[1] = 200; /* immutable time */
    manage->New.Data.Cnt[2] = 0; /* image counter */

    manage->New.Data.Cnt[3] = 0; /* stealth */
    manage->New.Data.Cnt[4] = 0;  /* shot counter */

    manage->New.Grp.image = Boss6Image;
    return NewObj(MEnemy,BossAct6,BossHit1,DrawImage);
}

DelAtt BossAct6(ObjData *my)
{
    if (my->Cnt[0] < my->Cnt[1])
    {/* appears slowly */
	my->Y++;
	my->Cnt[0]++;
	return NoneDel;
    }

    if (manage->BossTime <= 0)
    {
      if (my->Cnt[3] == 50)
      {
        if ((player->Ships <= 0) && (manage->player[0]->Data.Cnt[3] != 0))
          return NoneDel;
        else
          return BossDel;
      }
    }

    if (manage->BossTime >= 1)
      (manage->BossTime)--;

    if (my->Cnt[3] == 0)
    {
	my->hitAtt = MEnemy;
        my->hitMask = MPlayer | MPShot;
	my->image = 0;
	switch (my->Cnt[4]%10)
	{
	  case 0:
	    if (manage->Loop > 2)
	    {
		RingToPoint(my->X-128,my->Y+32,manage->player[0]->Data.X,manage->player[0]->Data.Y,rand()%5+3);
		RingToPoint(my->X-128,my->Y+32,manage->player[0]->Data.X,manage->player[0]->Data.Y,rand()%8+3);
		RingToPoint(my->X-128,my->Y+32,manage->player[0]->Data.X,manage->player[0]->Data.Y,rand()%3+5);
	    }
	    RingToAngle(my->X-128,my->Y+32,80,8);
	    RingToAngle(my->X-128,my->Y+32,90,8);
	    RingToAngle(my->X-128,my->Y+32,100,8);
	    break;
	  case 2:
	    RingToAngle(my->X,my->Y+32,80,8);
	    RingToAngle(my->X,my->Y+32,90,8);
	    RingToAngle(my->X,my->Y+32,100,8);
	    break;
	  case 4:
	    if (manage->Loop > 2)
	    {
		RingToPoint(my->X+128,my->Y+32,manage->player[0]->Data.X,manage->player[0]->Data.Y,rand()%5+3);
		RingToPoint(my->X+128,my->Y+32,manage->player[0]->Data.X,manage->player[0]->Data.Y,rand()%8+3);
		RingToPoint(my->X+128,my->Y+32,manage->player[0]->Data.X,manage->player[0]->Data.Y,rand()%3+5);
	    }
	    RingToAngle(my->X+128,my->Y+32,80,8);
	    RingToAngle(my->X+128,my->Y+32,90,8);
	    RingToAngle(my->X+128,my->Y+32,100,8);
	    break;
	  case 6:
	    HomingShot(my->X-90,my->Y-20,-15,-10);
	    HomingShot(my->X-60,my->Y-20,0,-10);
	    HomingShot(my->X+60,my->Y-20,0,-10);
	    HomingShot(my->X+90,my->Y-20,15,-10);
	    break;
	  case 8:
            LaserShot(my->X-120 +rand()%240,my->Y,20);
	    LaserShot(my->X-120,my->Y,20);
	    LaserShot(my->X+120,my->Y,20);
            if (manage->Loop == 1)
              LaserShot(my->X, my->Y, 20);
            else if (manage->Loop == 2)
            {
              LaserShot(my->X-30,my->Y,20);
              LaserShot(my->X+30,my->Y,20);
            }
            else
            {
              LaserShot(my->X -60, my->Y, 20);
              LaserShot(my->X, my->Y, 20);
              LaserShot(my->X +60,my->Y,20);
            }
	    break;
	}
        if (manage->Loop >2)
          if (my->Cnt[4] %2 == 0)
            if ((manage->player[0]->Data.X < my->X -120)
                ||(manage->player[0]->Data.X > my->X +120)
                ||(manage->player[0]->Data.Y < my->Y))
              ShotToPoint(my->X,my->Y,manage->player[0]->Data.X,manage->player[0]->Data.Y, 18);
	my->Cnt[4]++;
    }
    else
    {
	my->Cnt[3]--;
	if (my->Cnt[3] < 20)
	{
	    if (my->Cnt[3]%2 == 0)
		my->image = 0;
	    else
		my->image = 1;
	}
    }

    if (my->Cnt[4] == 45)
    {
        ShotToAngle(my->X -120, my->Y, 105, 25);
        ShotToAngle(my->X -120, my->Y, 115, 25);
        ShotToAngle(my->X -120, my->Y, 125, 25);
        ShotToAngle(my->X -120, my->Y, 135, 25);
        ShotToAngle(my->X -120, my->Y, 145, 25);
        ShotToAngle(my->X +120, my->Y, 75, 25);
        ShotToAngle(my->X +120, my->Y, 65, 25);
        ShotToAngle(my->X +120, my->Y, 55, 25);
        ShotToAngle(my->X +120, my->Y, 45, 25);
        ShotToAngle(my->X +120, my->Y, 35, 25);
        if (manage->Loop >1)
        {
          ShotToAngle(my->X -120, my->Y, 155, 25);
          ShotToAngle(my->X -120, my->Y, 165, 25);
          ShotToAngle(my->X -120, my->Y, 175, 25);
          ShotToAngle(my->X +120, my->Y, 25, 25);
          ShotToAngle(my->X +120, my->Y, 15, 25);
          ShotToAngle(my->X +120, my->Y, 5, 25);
        }
    }

    if (my->Cnt[4] >= 50)
    {
	my->Cnt[3] = 50;
	my->Cnt[4] = 0;
        my->hitAtt = 0;
        my->hitMask = 0;
	my->image = 1;
        my->showDamegeTime = 0;

        ShotToAngle(my->X -120, my->Y, 100, 25);
        ShotToAngle(my->X -120, my->Y, 110, 25);
        ShotToAngle(my->X -120, my->Y, 120, 25);
        ShotToAngle(my->X -120, my->Y, 130, 25);
        ShotToAngle(my->X -120, my->Y, 140, 25);
        ShotToAngle(my->X +120, my->Y, 80, 25);
        ShotToAngle(my->X +120, my->Y, 70, 25);
        ShotToAngle(my->X +120, my->Y, 60, 25);
        ShotToAngle(my->X +120, my->Y, 50, 25);
        ShotToAngle(my->X +120, my->Y, 40, 25);
        if (manage->Loop >1)
        {
          ShotToAngle(my->X -120, my->Y, 150, 25);
          ShotToAngle(my->X -120, my->Y, 160, 25);
          ShotToAngle(my->X -120, my->Y, 170, 25);
          ShotToAngle(my->X +120, my->Y, 30, 25);
          ShotToAngle(my->X +120, my->Y, 20, 25);
          ShotToAngle(my->X +120, my->Y, 10, 25);
        }

	my->X = rand()%FieldW;
	my->Y = rand()%((FieldH -(FieldH%2))/2);
    }

    return NoneDel;
}

int NewBoss7(void)
{
    manage->New.Data.hitAtt = MEnemy;
    manage->New.Data.hitMask = MPlayer | MPShot;

    manage->New.Data.HP = 150;
    manage->New.Data.Point = 8000;
    manage->New.Data.EnemyAtt = BossDel;

    manage->New.Data.Width = 32;
    manage->New.Data.Height = 32;

    manage->New.Data.X = FieldW/2;
    manage->New.Data.Y = 1 - manage->New.Data.Height/2;

    manage->New.Data.Cnt[0] = 0;
    manage->New.Data.Cnt[1] = 0; /* appears slowly */
    manage->New.Data.Cnt[2] = 0; /* image counter */

    do {
	manage->New.Data.Cnt[3] = rand()%30 - 15; /* x move */
    }while (abs(manage->New.Data.Cnt[3]) < 5);
    do {
	manage->New.Data.Cnt[4] = rand()% 10 + 5; /* y move */
    }while (abs(manage->New.Data.Cnt[4]) < 5);

    manage->New.Data.Cnt[5] = 0;  /* shot counter */

    /* "appeared" counter */
    manage->New.Data.Cnt[6] = 0;

    manage->New.Grp.image = EBoundImage;
    return NewObj(MEnemy,BossAct7,BossHit1,DrawImage);
}

DelAtt BossAct7(ObjData *my)
{
  int i;
    if (manage->BossTime >= 1)
      (manage->BossTime)--;

    my->image++;
    if (my->image >= 8)
        my->image = 0;

    if (manage->BossTime >= 1)
    {
      if (manage->Loop >1)
      {
        if (my->Cnt[5] % 20 == 0)
        {
          for (i = 0; i<360; i+= (90 - (90 % manage->Loop))/ manage->Loop)
            ShotToAngle(my->X, my->Y, i, manage->Loop * 2);
        }
      }

      if (my->Cnt[5] % 30 == 0)
      {
        BoundShot(my->X,my->Y,rand()%30-15,rand()%30-15, 5);
      }
    }
    

    (my->Cnt[5])++;

    if (my->Cnt[5] >= 60)
      my->Cnt[5] = 0;

    if ((my->Cnt[6] != 0) && (manage->BossTime >= 1))
    {
      if ((my->X+my->Cnt[3]>FieldW) || (my->X+my->Cnt[3]<0))
        my->Cnt[3] = my->Cnt[3]*(-1);
      if ((my->Y+my->Cnt[4]>FieldH) || (my->Y+my->Cnt[4]<0))
        my->Cnt[4] = my->Cnt[4]*(-1);
    }

    if ((my->X < 0 - my->Width/2) || (my->X > FieldW + my->Width/2))
    {
      if ((player->Ships <= 0) && (manage->player[0]->Data.Cnt[3] != 0))
        return NoneDel;
      else
        return BossDel;
    }
    if ((my->Y < 0 - my->Height/2) || (my->Y > FieldH + my->Height/2))
    {
      if ((player->Ships <= 0) && (manage->player[0]->Data.Cnt[3] != 0))
        return NoneDel;
      else
        return BossDel;
    }
      
    my->X += my->Cnt[3];
    my->Y += my->Cnt[4];

    if ((my->Cnt[6] == 0) && (my->X >= 0) && (my->X <= FieldW)
        && (my->Y >= 0) && (my->Y < FieldH))
      my->Cnt[6] = 1;

    return NoneDel;
}

int NewBoss8(void)
{
    manage->New.Data.hitAtt = MEnemy;
    manage->New.Data.hitMask = MPlayer | MPShot;

    manage->New.Data.X = FieldW/2;
    manage->New.Data.Y = -80;

    manage->New.Data.HP = 600;
    manage->New.Data.Point = 10000;
    manage->New.Data.EnemyAtt = BossDel;

    manage->New.Data.Width = 200;
    manage->New.Data.Height = 180;

    manage->New.Data.Cnt[0] = 0;
    manage->New.Data.Cnt[1] = 200; /* appears slowly */
    manage->New.Data.Cnt[2] = 0; /* image counter */
    manage->New.Data.Cnt[3] = 10; /* x move */
    manage->New.Data.Cnt[4] = 0; /* y move */
    manage->New.Data.Cnt[5] = 0;  /* shot counter */
    manage->New.Data.Cnt[6] = rand()%4; /* mode */
    manage->New.Data.Cnt[7] = 0; /* mode counter */
    manage->New.Data.Cnt[8] = 0; /* after-death counter */

    manage->New.Grp.image = Boss7Image;
    return NewObj(MEnemy,BossAct8,BossHit8,DrawImage);
}

DelAtt BossAct8(ObjData *my)
{
    if (my->Cnt[0] < my->Cnt[1])
    {/* appears slowly */
	my->Y++;
	my->Cnt[0]++;
	return NoneDel;
    }

    if (my->kill == True)
    {
      /* the boss is dead */
      return BossAct8_dead(my);
    }

    /* the boss is alive */
    if (manage->BossTime <= 0)
    {
      if (my->Y < 0 - my->Height/2 - 50)
      {
        if ((player->Ships > 0) ||(manage->player[0]->Data.Cnt[3] == 0))
          return BossDel;
        else
          return NoneDel;
      }
      else
      {
        (my->Y) -= 4;
        return NoneDel;
      }
    }

    (manage->BossTime)--;

    if (my->Cnt[5] > 8)
	my->Cnt[5] = 0;
    else
	my->Cnt[5]++;

    if ((my->Cnt[5]%2==0) && ((my->Cnt[6] ==0) || (my->Cnt[6] == 2)
                              || (my->Cnt[6] == 3)))
    {
	RingToAngle(my->X, my->Y-60, rand()%50+70, rand()%15+5);
	RingToAngle(my->X, my->Y-60, rand()%20+80, rand()%10+10);
        /*
	if (manage->Loop > 2)
	    RingToAngle(my->X, my->Y-60, rand()%20+80, rand()%5+15);
        */
    }

    if (my->Cnt[6] != 4)
    {
	if (my->X>manage->player[0]->Data.X && my->X>my->HarfW)
	    my->X--;
	else if (my->X<manage->player[0]->Data.X && my->X<(FieldW-my->HarfW))
	    my->X++;
    }

    if (my->Cnt[6] == 0)
    {
      return BossAct8_rand_shot(my);
    }
    else if (my->Cnt[6] == 1)
    {
      return BossAct8_ring_shot(my);
    }
    else if (my->Cnt[6] == 2)
    {
      return BossAct8_bound_shot(my);
    }
    else if (my->Cnt[6] == 3)
    {
      return BossAct8_homing_shot(my);
    }
    else if (my->Cnt[6] == 4)
    {
      return BossAct8_charge_shot(my);
    }
    else if (my->Cnt[6] == 5)
    {
      return BossAct8_n_way_shot(my);
    }

    return NoneDel;
}

DelAtt BossHit8(ObjData *my, ObjData *your)
{
  int temp = your->Attack;
    if (my->Cnt[0] < my->Cnt[1])
	return NoneDel;
    /*
    if (my->Cnt[6] == 4)
	return NoneDel;
    */
    /* immutable in above cases */
    if (my->HP < your->Attack)
      temp = my->HP;

    my->HP -= temp;
    player->Rec[0].score += temp;

    if (my->HP <= 0)
    {
       player->Rec[0].score -= 1;     
       my->showDamegeTime = 0;
        
       my->kill = True;
    }
    else
    {
      my->showDamegeTime = 15;
    }
    
    return NoneDel;
}

static DelAtt BossAct8_dead(ObjData *my)
{
  int i;
  int j;
  /* the boss is dead */
  NewLargeBomb(my->X+rand()%230-115,my->Y+rand()%180-90);
  NewLargeBomb(my->X+rand()%230-115,my->Y+rand()%180-90);

  my->Cnt[8]++;
  if ((player->Ships <= 0) && (manage->player[0]->Data.Cnt[3] != 0))
    if (my->Cnt[8] >= 300)
      my->Cnt[8] = 0;
  
  if (my->Cnt[8] %3 == 0)
  {
    if (my->X > manage->player[0]->Data.X)
      (my->X)--;
    else if (my->X < manage->player[0]->Data.X)
      (my->X)++;
    if (my->Y > manage->player[0]->Data.Y)
      (my->Y)--;
    else if (my->Y < manage->player[0]->Data.Y)
      (my->Y)++;
  }
  
  if (my->Cnt[8] %30 == 0)
  {
    for (i=0; i<24; i++)
      ShotToAngle(my->X,my->Y,i*15, 7);
  }
  else if (my->Cnt[8] %30 == 15)
  {
    for (i=0; i<24; i++)
      ShotToAngle(my->X,my->Y,i*15 + 7,7);
  }
  if (my->Cnt[8] %13 == 0)
  {
    ShotToPoint(my->X,my->Y,manage->player[0]->Data.X,manage->player[0]->Data.Y, manage->Loop *5);
    if (manage->Loop >2)
    {
      BoundShot(my->X, my->Y-60,rand()%20-10,rand()%4+10,4);
    }
  }
  if (my->Cnt[8] %5 == 0)
  {
    if (manage->Loop >1)
    {
      for (i=1; i< manage->Loop * 3; i++)
        ShotToAngle(my->X,my->Y,rand()%360, manage->Loop *3);
    }
  }
  
  if (my->Cnt[8] >= 300)
  {
    for (i=0; i<=10; i++)
      for (j=0; j<=10; j++)
        NewLargeBomb(50*i,65*j);
    return my->EnemyAtt;
  }
  else
    return NoneDel;
}

static DelAtt BossAct8_rand_shot(ObjData *my)
{
  int i;
  if (my->Cnt[7]% 10 == 0)
  {
    for (i = 1; i <= manage->Loop; i++)
    {
      ShotToAngle(my->X-60, my->Y-60, rand()%50 + 70, rand()%5+5);
      ShotToAngle(my->X+60, my->Y-60, rand()%50 + 70, rand()%5+5);
    }
    ShotToPoint(my->X,my->Y-60,manage->player[0]->Data.X,manage->player[0]->Data.Y,rand()%5+3);
  }

  BossAct8_next(my, 100);
  return NoneDel;
}

static DelAtt BossAct8_ring_shot(ObjData *my)
{
  if (my->Cnt[7]%2 == 0)
  {
    RingToPoint(my->X-60, my->Y+10, manage->player[0]->Data.X,manage->player[0]->Data.Y, rand()%10 + 20);
    RingToPoint(my->X+60, my->Y+10, manage->player[0]->Data.X,manage->player[0]->Data.Y, rand()%10 + 20);
    if (manage->Loop >1)
    {
      RingToAngle(my->X, my->Y - 60, 90, rand()%10 + 20);
      RingToAngle(my->X, my->Y - 60, 90, rand()%10 + 20);
    }
    if (manage->Loop >2)
    {
      RingToAngle(my->X-60, my->Y - 60, 90, rand()%10 + 20);
      RingToAngle(my->X+60, my->Y - 60, 90, rand()%10 + 20);
    }
  }

  BossAct8_next(my, 70);
  return NoneDel;
}

static DelAtt BossAct8_bound_shot(ObjData *my)
{
  if (manage->Loop <=2)
  {
    if (my->Cnt[7]  % 16 == 0)
      BoundShot(my->X+60,my->Y-60,rand()%20-10,rand()%4+10,2);
    else if (my->Cnt[7]  % 16 == 8)
      BoundShot(my->X-60,my->Y-60,rand()%20-10,rand()%4+10,2);
  }
  else
  {
    if (my->Cnt[7]  % 16 == 0)
      BoundShot(my->X+60,my->Y-60, (rand()%20-10)*2, (rand()%4+10)*2, 2);
    else if (my->Cnt[7]  % 16 == 8)
      BoundShot(my->X-60,my->Y-60, (rand()%20-10)*2 ,(rand()%4+10)*2, 2);
  }

  BossAct8_next(my, 100);
  return NoneDel;
}

static DelAtt BossAct8_homing_shot(ObjData *my)
{
  if (my->Cnt[7] %8 == 0)
  {
    HomingShot(my->X-60,my->Y-10,-10, 5);
    HomingShot(my->X+60,my->Y-10, 10, 5);
    if (manage->Loop > 2)
    {
      HomingShot(my->X-60,my->Y-10,-3, 10);
      HomingShot(my->X+60,my->Y-10, 3, 10);
    }
  }

  BossAct8_next(my, 100);
  return NoneDel;
}

static DelAtt BossAct8_charge_shot(ObjData *my)
{
  int i;
  if (my->Cnt[7] == 48 - manage->Loop *8)
  {
    for (i=0; i<24; i++)
      ShotToAngle(my->X,my->Y,i*15,15);
  }
  if (my->Cnt[7] == 53 - manage->Loop *8)
  {
    for (i=0; i<24; i++)
      ShotToAngle(my->X,my->Y,i*15+7,20);
  }
  if (my->Cnt[7] > 53 - manage->Loop *8)
  {
    LaserShot(my->X-120,my->Y-60,35);
    LaserShot(my->X-80 ,my->Y-60,35);
    LaserShot(my->X-40 ,my->Y-60,35);
    LaserShot(my->X    ,my->Y-60,35);
    LaserShot(my->X+40 ,my->Y-60,35);
    LaserShot(my->X+80 ,my->Y-60,35);
    LaserShot(my->X+120,my->Y-60,35);
  }

  BossAct8_next(my, 58 - manage->Loop *8);
  return NoneDel;
}

static DelAtt BossAct8_n_way_shot(ObjData *my)
{
  int xtemp;
  int ytemp;
  int speedtemp;
  if (my->Cnt[7] % 13 == 0)
  {
    xtemp = rand()%FieldW;
    ytemp = rand()%((FieldH -(FieldH%2))/2);
    speedtemp = rand()%7 + 5;

    if (manage->Loop == 1)
    {
      ShotToAngle(xtemp, ytemp, 50, speedtemp);
      ShotToAngle(xtemp, ytemp, 70, speedtemp);
      ShotToAngle(xtemp, ytemp, 90, speedtemp);
      ShotToAngle(xtemp, ytemp, 110, speedtemp);
      ShotToAngle(xtemp, ytemp, 130, speedtemp);
    }
    else if (manage->Loop == 2)
    {
      ShotToAngle(xtemp, ytemp, 51, speedtemp);
      ShotToAngle(xtemp, ytemp, 64, speedtemp);
      ShotToAngle(xtemp, ytemp, 77, speedtemp);
      ShotToAngle(xtemp, ytemp, 90, speedtemp);
      ShotToAngle(xtemp, ytemp, 103, speedtemp);
      ShotToAngle(xtemp, ytemp, 116, speedtemp);
      ShotToAngle(xtemp, ytemp, 129, speedtemp);
    }
    else
    {
      ShotToAngle(xtemp, ytemp, 50, speedtemp);
      ShotToAngle(xtemp, ytemp, 60, speedtemp);
      ShotToAngle(xtemp, ytemp, 70, speedtemp);
      ShotToAngle(xtemp, ytemp, 80, speedtemp);
      ShotToAngle(xtemp, ytemp, 90, speedtemp);
      ShotToAngle(xtemp, ytemp, 100, speedtemp);
      ShotToAngle(xtemp, ytemp, 110, speedtemp);
      ShotToAngle(xtemp, ytemp, 120, speedtemp);
      ShotToAngle(xtemp, ytemp, 130, speedtemp);
    }
  }

  BossAct8_next(my, 150);
  return NoneDel;
}

static void BossAct8_next(ObjData *my, int span)
{
  int modetemp = 0;
  if (my->Cnt[7] > span)
  {
    do
    {
      modetemp = rand()%6;
    }while (modetemp == my->Cnt[6]);
    my->Cnt[6] = modetemp;
    my->Cnt[7] = 0;
  }
  else
    my->Cnt[7]++;
}
