/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: player.c,v 1.12 2011/08/12 14:33:57 oohara Exp $ */

/* DEBUG is defined in config.h */
#include <config.h>
/* rand */
#include <stdlib.h>

/*
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/xpm.h>
*/

#include "image.h"
#include "xsoldier.h"
#include "manage.h"
#include "player.h"
#include "common.h"
#include "callback.h"
#include "extern.h"
#include "key.h"
#include "sin.h"
/* NewEnemy10 (power-up item) */
#include "enemy.h"

/* player object */
void NewPlayer(int x, int y)
{
    manage->player[0]->Data.hitAtt = MPlayer;
    manage->player[0]->Data.hitMask = MEnemy | MEShot | MItem;

    manage->player[0]->Data.used = True;
    manage->player[0]->Data.kill = False;
    manage->player[0]->Data.X = x;
    manage->player[0]->Data.Y = y;
    manage->player[0]->Data.Speed = 15;
    manage->player[0]->Data.Attack = 0;
    manage->player[0]->Data.Width = 16;
    manage->player[0]->Data.Height = 16;
    manage->player[0]->Data.HarfW = manage->player[0]->Data.Width/2;
    manage->player[0]->Data.HarfH = manage->player[0]->Data.Height/2;
    manage->player[0]->Data.image = 0;

    manage->player[0]->Data.Cnt[0] = 0; /* auto shot counter */
    manage->player[0]->Data.Cnt[1] = 0; /* image counter */
    manage->player[0]->Data.Cnt[2] = 60; /* immutable counter */
    manage->player[0]->Data.Cnt[3] = 0; /* death coounter */
    manage->player[0]->Data.Cnt[4] = False; /* transparent flag */
    manage->player[0]->Data.Cnt[5] = 1; /* weapon counter */
    manage->player[0]->Data.Cnt[6] = manage->start_power; /* power counter */
    
    manage->player[0]->Grp.image = PlayerImage;
    /*
    manage->player[0]->Grp.Width = manage->player[0]->Grp.image[0]->width;
    manage->player[0]->Grp.Height = manage->player[0]->Grp.image[0]->height;
    */
    manage->player[0]->Grp.Width = 32;
    manage->player[0]->Grp.Height = 32;
    manage->player[0]->Grp.HarfW = manage->player[0]->Grp.Width/2;
    manage->player[0]->Grp.HarfH = manage->player[0]->Grp.Height/2;
    
    manage->player[0]->Action = PlayerAction;
    manage->player[0]->Hit = PlayerHit;
    manage->player[0]->Realize = DrawImage;
    manage->player[0]->Data.notShootingTime = 0;

    manage->PlayerNum++;
}

void RestartPlayer(int x, int y)
{
    manage->player[0]->Data.hitAtt = MPlayer;
    manage->player[0]->Data.hitMask = MEnemy | MEShot | MItem;
    manage->player[0]->Data.used = True;
    manage->player[0]->Data.kill = False;
    manage->player[0]->Data.X = x;
    manage->player[0]->Data.Y = y;

    manage->player[0]->Data.Cnt[0] = 0; /* auto shot counter */
    manage->player[0]->Data.Cnt[1] = 0; /* image counter */
    manage->player[0]->Data.Cnt[2] = 60; /* immutable counter */
    manage->player[0]->Data.Cnt[3] = 0; /* death counter */
    manage->player[0]->Data.Cnt[4] = False; /* transparent flag */
    manage->player[0]->Data.notShootingTime = 0;
    
    manage->PlayerNum++;
}

DelAtt PlayerAction(ObjData *my)
{
    /* you can change speed at any time */
    if (keymask & SpeedUP)
    {
	if (my->Speed < 30)
	    my->Speed += 3;
	keymask -= SpeedUP;
    }
    if (keymask & SpeedDOWN)
    {
	if (my->Speed > 6)
	    my->Speed -= 3;
	keymask -= SpeedDOWN;
    }

    if (joymask & SpeedUP)
    {
	if (my->Speed < 30)
	    my->Speed += 3;
    }
    if (joymask & SpeedDOWN)
    {
	if (my->Speed > 6)
	    my->Speed -= 3;
    }

    /* your action is over if you are dead */
    if (my->kill == True)
    {
	my->Cnt[3]++;
	if (my->Cnt[3] == 30)
	    return NullDel;

	NewLargeBomb(my->X+rand()%20-10,my->Y+rand()%20-10);
	return NoneDel;
    }

    /* move */
    if (keymask & Up || joymask & Up)
	my->Y -= my->Speed;
    if (keymask & Down || joymask & Down)
	my->Y += my->Speed;
    if (keymask & Left || joymask & Left)
    {
	my->X -= my->Speed;
	my->Cnt[1] -= 2;
    }
    if (keymask & Right || joymask & Right)
    {
	my->X += my->Speed;
	my->Cnt[1] += 2;
    }

    /* choose image */
    if (my->Cnt[1] > 0)
	my->Cnt[1]--;
    else if (my->Cnt[1] < 0)
	my->Cnt[1]++;

    if (my->Cnt[1] > 10)
	my->image = 4;
    else if (my->Cnt[1] > 0)
	my->image = 3;
    else if (my->Cnt[1] < -10)
	my->image = 2;
    else if (my->Cnt[1] < 0)
	my->image = 1;
    else
	my->image = 0;

    if (my->Cnt[2] != 0)
    {
	if (my->Cnt[4] == False)
	{
	    my->image = 5;
	    my->Cnt[4] = True;
	}
	else
	    my->Cnt[4] = False;
    }

    /* boundary check for moving */
    if (my->X - 16 < 0)
	my->X = 0 + 16;
    else if (my->X + 16 > FieldW)
	my->X = FieldW - 16;
    if (my->Y - 16 < 0)
	my->Y = 0 + 16;
    else if (my->Y + 16 > FieldH)
	my->Y = FieldH - 16;

    if (my->Cnt[2] > 0)
	my->Cnt[2]--;

    if (keymask & Shot || joymask & Shot)
    {
      my->notShootingTime = 5;
	if (my->Cnt[0] == 0)
	{
	    switch (my->Cnt[5])
	    {
	      case 1:
                /* how often you can shoot */
		if (my->Cnt[6] >= 30)
		    my->Cnt[0] = 2;
		else
		    my->Cnt[0] = 3;

		if (my->Cnt[6] >= 20)
		{
                  PlayerShot1(my->X-10,my->Y,35,90, my->Cnt[6]);
                  PlayerShot1(my->X+10,my->Y,35,90, my->Cnt[6]);
		}
                else if (my->Cnt[6] >= 10)
                  PlayerShot1(my->X, my->Y,35,90, my->Cnt[6]);

		if (my->Cnt[6] >= 15)
		{
                  PlayerShot1(my->X-40,my->Y+10,35,270, my->Cnt[6]);
                  PlayerShot1(my->X-10,my->Y,35,270, my->Cnt[6]);
                  PlayerShot1(my->X+10,my->Y,35,270, my->Cnt[6]);
                  PlayerShot1(my->X+40,my->Y+10,35,270, my->Cnt[6]);
		}
                else if (my->Cnt[6] >= 5)
                {
                  PlayerShot1(my->X-25,my->Y+10,35,270, my->Cnt[6]);
                  PlayerShot1(my->X, my->Y,35,270, my->Cnt[6]);
                  PlayerShot1(my->X+25, my->Y+10,35,270, my->Cnt[6]);
                }
                else
                {
                  PlayerShot1(my->X-10,my->Y,35,270, my->Cnt[6]);
                  PlayerShot1(my->X+10,my->Y,35,270, my->Cnt[6]);
                }
                
		break;
	      case 2:
                /* how often you can shoot */
		if (my->Cnt[6] >= 25)
		    my->Cnt[0] = 1;
		else if (my->Cnt[6] >= 19)
		    my->Cnt[0] = 2;
		else
		    my->Cnt[0] = 3;
                PlayerShot2(my->X,my->Y,25,255);
		PlayerShot2(my->X,my->Y,25,270);
		PlayerShot2(my->X,my->Y,25,285);
                if (my->Cnt[6] >= 15)
		{
		    PlayerShot2(my->X,my->Y,25,0);
		    PlayerShot2(my->X,my->Y,25,180);
		}
                if (my->Cnt[6] >= 11)
		{
		    PlayerShot2(my->X,my->Y,25,225);
		    PlayerShot2(my->X,my->Y,25,315);
		}
		if (my->Cnt[6] >= 7)
		{
		    PlayerShot2(my->X,my->Y,25,60);
		    PlayerShot2(my->X,my->Y,25,120);
		}
		if (my->Cnt[6] >= 3)
		{
		    PlayerShot2(my->X,my->Y,25,240);
		    PlayerShot2(my->X,my->Y,25,300);
		}

		break;
	      case 3:
                /* how often you can shoot */
		if (my->Cnt[6] >= 30)
		    my->Cnt[0] = 3;
		else
		    my->Cnt[0] = 4;
		if (my->Cnt[6] >= 10)
		{
		    PlayerShot3(my->X,my->Y-10,0, my->Cnt[6]);
		    PlayerShot3(my->X-10,my->Y-10,-1, my->Cnt[6]);
		    PlayerShot3(my->X+10,my->Y-10,1, my->Cnt[6]);
		}
                else if (my->Cnt[6] >= 5)
		{
		    PlayerShot3(my->X-10,my->Y-10,0, my->Cnt[6]);
		    PlayerShot3(my->X+10,my->Y-10,0, my->Cnt[6]);
		}
		else
		    PlayerShot3(my->X,my->Y-10,0, my->Cnt[6]);
		break;
	    }
	}
	else
	    my->Cnt[0]--;
    }
    else
    {
      if (my->notShootingTime <= 0)
      {
        /* releasing the shoot button gives you a score */
        player->Rec[0].score += (manage->EnemyNum) * 3;
      }
      else
        (my->notShootingTime)--;
      
      my->Cnt[0] = 0;
    }
    
    return NoneDel;
}

DelAtt PlayerHit(ObjData *my, ObjData *your)
{
    if (your->hitAtt == MItem)
    {
	if (your->Cnt[0] == 0)
	    my->Cnt[6] += 2;
	else
        {
          my->Cnt[6]++;
          my->Cnt[5] = your->Cnt[0];
        }
        
	for(; my->Cnt[6] > 45; (my->Cnt[6])--)
        {
          player->Rec[0].score += 1000 * manage->Loop;
          (manage->Level)++;
        }
          
        if (manage->Level > MaxLevel)
          manage->Level = MaxLevel;
        if (manage->Level < 0)
          manage->Level = 0;

        if (manage->flag_maxlevel == True)
          manage->Level = MaxLevel;

	return NoneDel;
    }

    if (my->Cnt[2] == 0)
    {
	my->kill = True;
	my->image = 5;
        manage->Level -= 5;
        if (manage->Level > MaxLevel)
          manage->Level = MaxLevel;
        if (manage->Level < 0)
          manage->Level = 0;

        if (manage->flag_maxlevel == True)
          manage->Level = MaxLevel;
    }
    return NoneDel;
}

/* player shot */
void PlayerShot1(int x, int y, int speed, int angle, int attack)
{
    manage->New.Data.hitAtt = MPShot;
    manage->New.Data.hitMask = MEnemy;

    manage->New.Data.X = x;
    manage->New.Data.Y = y;
    if (attack >= 25)
      manage->New.Data.Attack = 3;
    else
      manage->New.Data.Attack = 2;
    
    manage->New.Data.Speed = speed;
    manage->New.Data.Angle = angle;
    manage->New.Data.EnemyAtt = NullDel;
    manage->New.Data.Width = 10;
    manage->New.Data.Height = 26;

    manage->New.Data.Cnt[0] = x << 8;
    manage->New.Data.Cnt[1] = y << 8;
    manage->New.Data.Cnt[2] = icos(angle);
    manage->New.Data.Cnt[3] = isin(angle);

    manage->New.Data.Cnt[4] = 0;

    manage->New.Grp.image = PShot1Image;

    NewObj(MPShot,PlayerShotAct1,PlayerShotHit1,DrawImage);
}

void PlayerShot2(int x, int y, int speed, int angle)
{
    manage->New.Data.hitAtt = MPShot;
    manage->New.Data.hitMask = MEnemy;

    manage->New.Data.X = x;
    manage->New.Data.Y = y;
    manage->New.Data.Attack = 1;
    manage->New.Data.Speed = speed;
    manage->New.Data.Angle = angle;
    manage->New.Data.EnemyAtt = NullDel;
    manage->New.Data.Width = 12;
    manage->New.Data.Height = 16;

    manage->New.Data.Cnt[0] = x << 8;
    manage->New.Data.Cnt[1] = y << 8;
    manage->New.Data.Cnt[2] = icos(angle);
    manage->New.Data.Cnt[3] = isin(angle);

    manage->New.Data.Cnt[4] = 0;

    manage->New.Grp.image = PShot2Image;

    NewObj(MPShot,PlayerShotAct1,PlayerShotHit1,DrawImage);
}

DelAtt PlayerShotAct1(ObjData *my)
{
    if (my->Cnt[4] >= 1)
	return NullDel;

    /* 2^8 = 256 */
    my->Cnt[0] += my->Cnt[2]*my->Speed;
    my->X = my->Cnt[0] / 256;
    my->Cnt[1] += my->Cnt[3]*my->Speed;
    my->Y = my->Cnt[1] / 256;

    if ((my->X<0) || (my->X>FieldW))
	return NullDel;
    if ((my->Y<0) || (my->Y>FieldH))
	return NullDel;

    return NoneDel;
}

DelAtt PlayerShotHit1(ObjData *my, ObjData *your)
{
    if (my->Cnt[4] >= 1)
	return NullDel;

    my->Cnt[4]++;
    my->hitMask = 0;
    my->image = 1;
    return NoneDel;
}

void PlayerShot3(int x, int y, int inertX, int attack)
{
    manage->New.Data.hitAtt = MPShot;
    manage->New.Data.hitMask = MEnemy;

    manage->New.Data.X = x;
    manage->New.Data.Y = y;
    if (attack >= 22)
      manage->New.Data.Attack = 5;
    else if (attack >= 16)
      manage->New.Data.Attack = 4;
    else
      manage->New.Data.Attack = 3;
    manage->New.Data.EnemyAtt = NullDel;
    manage->New.Data.Width = 8;
    manage->New.Data.Height = 16;

    manage->New.Data.inertX = inertX;
    manage->New.Data.inertY = 5;
    manage->New.Data.Cnt[4] = 0;

    manage->New.Grp.image = PShot3Image;

    NewObj(MPShot,PlayerShotAct3,PlayerShotHit3,DrawImage);
}

DelAtt PlayerShotAct3(ObjData *my)
{
    if (my->Cnt[4] != 0)
    {
      /* delete the explosion */
	if (my->Cnt[4] >= 4)
	    return NullDel;
        /* no longer checks collision */
	if (my->Cnt[4] == 3)
          /*
	    my->Attack = 0;
          */
          my->hitMask = 0;
	if (my->Cnt[4] == 1)
	    my->image = 2;
	my->Cnt[4]++;
    }

    /* have not exploded yet */
    if (my->Cnt[4] == 0)
    {
	my->inertY++;
	my->X += my->inertX;
	my->Y -= my->inertY;
    }

    if ((my->X<0) || (my->X>FieldW))
	return NullDel;
    if ((my->Y<0) || (my->Y>FieldH))
	return NullDel;

    return NoneDel;
}

DelAtt PlayerShotHit3(ObjData *my, ObjData *your)
{
    if (my->Cnt[4] >= 4)
	return NullDel;
    else if (my->Cnt[4] == 0)
      /* explode */
    {
      my->Attack = 1;

	my->Width = 80;
	my->HarfW = my->Width / 2;
	
	my->Height = 80;
	my->HarfH = my->Height / 2;
	
	my->Cnt[4]++;
	my->image = 1;
	return NoneDel;
    }
    return NoneDel;
}

void PlayerLosePower(void)
{
  int i = 0;
  
  if (manage->player[0]->Data.Cnt[6] > 30)
    manage->player[0]->Data.Cnt[6] = 30;
  
  if (manage->player[0]->Data.Cnt[6] < manage->start_power + 10)
    return;
  else if (manage->player[0]->Data.Cnt[6] < manage->start_power + 25)
  {
    manage->player[0]->Data.Cnt[6] -= 5;
    i = 3;
  }
  else
  {
    manage->player[0]->Data.Cnt[6] -= 10;
    i = 6;
  }

  if (manage->player[0]->Data.Cnt[6] < 0)
    /* should not happen */
    manage->player[0]->Data.Cnt[6] = 0;

  for (; i >= 1; i--)
    /* the Y coordinate is intentionally subtracted */
    NewEnemy10(manage->player[0]->Data.X + rand() % 50 - 25,
               manage->player[0]->Data.Y - rand() % 20 - 10);
}
