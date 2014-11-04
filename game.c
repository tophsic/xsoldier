/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: game.c,v 1.34 2009/11/08 06:21:24 oohara Exp $ */

/* DEBUG and JSTK are defined in config.h */
#include <config.h>
/* pause */
#include <unistd.h>
/* rand */
#include <stdlib.h>
/* strlen */
#include <string.h>

#include <stdio.h>
/*
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
*/

#include "image.h"
#include "xsoldier.h"
#include "player.h"
#include "boss.h"
#include "enemy.h"
#include "extern.h"
#include "key.h"
#include "star.h"
/* ClearEnemyShotManage, ClearManage, DelObj */
#include "manage.h"
#include "graphic.h"
#include "input.h"

#ifdef JSTK
#include "joystick.h"
#endif

/* DamageHit, LargeDamageHit
 * DrawRec if DEBUG
 */
#include "callback.h"
#include "game.h"

/* local functions */
static void DrawInfo(void);

static void do_actions(void);
static void collision_detection(void);

static int shoot_down_bonus(int percent, int loop, int stage);
/*
static int perfect_bonus(int loop, int stage);
*/

int mainLoop(void)
{
    int obj; /* loop counter */

    int ocheck; /* counter for already checked objects */


    int oneUp = 0; /* 1up counter */

    /* functions of normal enemies, weak ones first */
    int (*NewEnemy[])(int x, int y) = 
    {
	NewEnemy1,NewEnemy2,NewEnemy3,NewEnemy4,NewEnemy5,
	NewEnemy6,NewEnemy7,NewEnemy8,NewEnemy9
    };

    /* functions of end-of-stage bosses */
    int (*NewBoss[])(void) =
    {
	NewBoss1,NewBoss2,NewBoss3,NewBoss4,NewBoss5,
	NewBoss6,NewBoss7,NewBoss8
    };

    /* number of normal enemies in each stage */    
    int StageObj[] = 
    {
	80,80,100,100,120,120,140,140
    };

    char StageName[][16] = 
    {
	"Stage 1","Stage 2","Stage 3","Stage 4","Stage 5",
	"Stage 6","Stage 7","Final Stage",
	"All Clear!"
    };
    
    /* add the player ship to the table */
    NewPlayer(FieldW/2,FieldH - 32);
#ifndef HAVE_LIBSDL
    XFlush(dpy);
#endif /* not HAVE_LIBSDL */
    while (1)
    {
        if (waittime && (signal_delivered==0))
	    pause();
        signal_delivered = 0;

        if (event_handle() == 0)
          return 0;

	if (keymask & Pause)
	{
          if (manage->flag_nopausemessage == False)
          {
            draw_string(235, 280, "Pause", strlen("Pause"));
            draw_string(180, 300, "Press [", strlen("Press ["));
            draw_string(230, 300, pauseKey, strlen(pauseKey));
            draw_string(245, 300, "] to resume game",
                        strlen("] to resume game"));
          }
          redraw_window();
          continue;
	}

	if (manage->Stage>MaxStage && manage->Appear>0)
	    break;
	
	if (manage->player[0]->Data.used == False)
	{
          /* the player is killed */
	    player->Rec[0].loop = manage->Loop;
	    player->Rec[0].stage = manage->Stage;
	    if (player->Ships > 0)
	    {
		player->Ships--;
                ClearEnemyShotManage(manage);
                PlayerLosePower();
		RestartPlayer(FieldW/2,FieldH - 32);
	    }
	}
	
	if (manage->BossKill == True)
	{
          /* the boss is killed, or it escaped */
          manage->Stage++;

          manage->Level += 5;

          if (manage->BossTime >=1)
          {
            /* the boss is dead */
	    player->Percent = (manage->StageShotDown/(double)manage->StageEnemy) * 100;
            /* note that manage->Stage is already incremented */
	    player->Rec[0].score += shoot_down_bonus(player->Percent, manage->Loop, manage->Stage);
	    if (player->Percent >= 100)
            {
              /*
              player->Rec[0].score += perfect_bonus(manage->Loop, manage->Stage);
              */
              manage->Level += 7;
            }
            
          }
          else
          {
            manage->Level -= 3;
          }
          if (manage->Level > MaxLevel)
            manage->Level = MaxLevel;
          if (manage->Level < 0)
            manage->Level = 0;

          if (manage->flag_maxlevel == True)
            manage->Level = MaxLevel;

	    ClearManage(manage);

	    ChangeStarParameter(StarPtn1,20);
	    ChangeStarParameter(StarPtn2,25);
	}

	if (manage->Appear >= 100)
	{
	    if ((manage->StageEnemy >= StageObj[manage->Stage-1]) && (manage->BossApp==False))
	    {
              /* the boss appears */
		if (NewBoss[manage->Stage-1]() != -1)
		{
                  manage->ZakoApp = False;
		    manage->BossApp = True;
		    manage->StageEnemy++;
                    if (manage->Stage == 8)
                      manage->BossTime = 3000;
                    else
                      manage->BossTime = 2000;
		}
	    }
	    else if (manage->ZakoApp == True)
	    {
              /* normal enemy */
		if (NewEnemy[rand()%(manage->Stage+1)]((rand()%FieldW)+1,0) != -1)
		{
		    manage->StageEnemy++;
		}
	    }
	    /* how often normal enemies appear */
            manage->Appear = 89;
	}
	else
	    manage->Appear++;

        do_actions();
	
        collision_detection();
        

	/* the player gets 1up? */
	if (player->Rec[0].score >= player->Next)
	{
	    player->Next += EVERY1UP;
	    player->Ships++;

            /* counter to display 1up message */
	    oneUp = 1;
	}

        /* draw the window */
        clear_window();

        /* pixmaps for the background */
	DrawStar(StarPtn1);
	DrawStar(StarPtn2);
	/* pixmaps for objects */
	for (obj=0,ocheck=0; (obj<manage->EnemyMax && ocheck<manage->EnemyNum); obj++)
	{
	    if (manage->enemy[obj]->Data.used == True)
	    {
		manage->enemy[obj]->Realize(&(manage->enemy[obj]->Data),&(manage->enemy[obj]->Grp));
		ocheck++;
	    }
	}
	for (obj=manage->PlayerMax-1,ocheck=0; (obj>=0 && ocheck<manage->PlayerNum); obj--)
	{
	    if (manage->player[obj]->Data.used == True)
	    {
		manage->player[obj]->Realize(&(manage->player[obj]->Data),&(manage->player[obj]->Grp));
		ocheck++;
	    }
	}
        if (player->Rec[0].score >= 10000000)
          player->Rec[0].score = 10000000;
        
	/* score and other stuff */
	DrawInfo();

	/* yet more misc stuff */
	if (oneUp != 0)
	{
	    if (oneUp%4 > 1)
              draw_string(440, 620, "1UP", strlen("1UP"));
	    oneUp++;
	    if (oneUp > 50)
		oneUp = 0;
	}

	if (manage->player[0]->Data.kill==True && player->Ships==0)
          draw_string(230, 300, "Game Over", strlen("Game Over"));

	if (manage->Appear < 0)
	{
	    char Percent[32];
	    char Bonus[32];
	    char Perfect[32];

	    if (manage->showShootDown != 0)
	    {
              /* shoot down bonus message */
              if (manage->BossTime >= 1)
              {
		sprintf(Percent,"shoot down %02d%%",player->Percent);
                draw_string(210, 370, Percent, strlen(Percent));


		sprintf(Bonus,"Bonus %d pts", shoot_down_bonus(player->Percent, manage->Loop, manage->Stage));
                draw_string(260 + manage->Appear*3 , 400,
                            Bonus, strlen(Bonus));

		if (player->Percent >= 100)
		{
		    sprintf(Perfect,"Perfect!!");
                    draw_string(170 - manage->Appear*3 , 420,
                                Perfect, strlen(Perfect));
		}
              }
              else
              {
                snprintf(Percent, 32, "the boss escaped");
                draw_string(200 ,370 ,Percent, strlen(Percent));
              }
              
	    }
            draw_string(230, 320, StageName[manage->Stage-1],
                        strlen(StageName[manage->Stage-1]));
	}
	if (manage->Appear == 0)
	{
	    ChangeStarParameter(StarPtn1,5);
	    ChangeStarParameter(StarPtn2,10);
	}

        redraw_window();
    }

    /* ending */
    return 1;
}

/* show score and other info */
static void DrawInfo(void)
{
    static char Score[64];
    static char Ships[16];
    static char Stage[16];
#ifdef DEBUG
    static char ObjectP[32];
    static char ObjectE[32];
    static char Loop[16];
    static char Level[16];
    static char Weapon[16];
    static char Pow[16];
    static char Speed[16];
    static char Enemy[16];
    static char EnemyKill[16];
#endif
    static char EnemyHP[5];
    static char BossTime[16];
    
    int i;

    sprintf(Score,"Score % 8d",player->Rec[0].score);
    sprintf(Stage,"Stage %2d",manage->Stage);
    sprintf(Ships,"Ships %3d",player->Ships);
#ifdef DEBUG
    sprintf(ObjectE,"Enemy Object %3d",manage->EnemyNum);
    sprintf(ObjectP,"Player Object %3d",manage->PlayerNum);
    sprintf(Loop,"Loop %2d",manage->Loop);
    sprintf(Level,"Level %3d",manage->Level);
    sprintf(Weapon,"Weapon %d",manage->player[0]->Data.Cnt[5]);
    sprintf(Pow,"Pow %2d",manage->player[0]->Data.Cnt[6]);
    sprintf(Speed,"Speed %2d",manage->player[0]->Data.Speed);
    sprintf(Enemy,"Enemy %3d",manage->StageEnemy);
    sprintf(EnemyKill,"EnemyKill %3d",manage->StageShotDown);
#endif

    draw_string(10, 20, Score, strlen(Score));
    draw_string(430, 20, Stage, strlen(Stage));
    draw_string(430, 640, Ships, strlen(Ships));
#ifdef DEBUG
    draw_string(10, 40, ObjectE, strlen(ObjectE));
    draw_string(10, 60, ObjectP, strlen(ObjectP));
    draw_string(10, 80, Level, strlen(Level));
    draw_string(10, 100, Enemy, strlen(Enemy));
    draw_string(10, 120, EnemyKill, strlen(EnemyKill));
    draw_string(430, 60, Loop, strlen(Loop));
    draw_string(430, 580, Weapon, strlen(Weapon));
    draw_string(430, 600, Pow, strlen(Pow));
    draw_string(430, 620, Speed, strlen(Speed));
#endif
    for (i = 0; i<manage->EnemyMax; i++)
      if (manage->enemy[i]->Data.used == True)
        if ((manage->enemy[i]->Hit == EnemyHit1)
            ||(manage->enemy[i]->Hit == DamageHit)
            ||(manage->enemy[i]->Hit == LargeDamageHit)
            ||(manage->enemy[i]->Hit == BossHit1)
            ||(manage->enemy[i]->Hit == BossHit8))
          if (manage->enemy[i]->Data.showDamegeTime >0)
          {
            snprintf(EnemyHP, 5, "%d",manage->enemy[i]->Data.HP);
            draw_string(manage->enemy[i]->Data.X, manage->enemy[i]->Data.Y,
                        EnemyHP, strlen(EnemyHP));
            (manage->enemy[i]->Data.showDamegeTime)--;
          }
    if (manage->BossApp == True)
    {
      snprintf(BossTime, 16, "Time %4d",manage->BossTime);
      draw_string(430, 40, BossTime, strlen(BossTime));
    }

#ifdef DEBUG
    for (i = 0; i<manage->EnemyMax; i++)
      if (manage->enemy[i]->Data.used == True)
        /* DrawRec does not use arg 2, so NULL will be enough */
        DrawRec(&(manage->enemy[i]->Data), NULL);
    for (i = 0; i<manage->PlayerMax; i++)
      if (manage->player[i]->Data.used == True)
        /* DrawRec does not use arg 2, so NULL will be enough */
        DrawRec(&(manage->player[i]->Data), NULL);
        
#endif /* DEBUG */



    
}

static void do_actions(void)
{
  int obj;
  DelAtt DelFlag;

  for (obj=manage->PlayerMax-1; obj>=0; obj--)
    manage->player[obj]->Data.shouldAct = manage->player[obj]->Data.used;
  for (obj=manage->PlayerMax-1; obj>=0; obj--)
  {
    if (manage->player[obj]->Data.shouldAct == True)
    {
      if (manage->player[obj]->Action(&(manage->player[obj]->Data)) == NullDel)
        DelObj(manage->player[obj]);
    }
  }
  
  for (obj=0; obj<manage->EnemyMax; obj++)
    manage->enemy[obj]->Data.shouldAct = manage->enemy[obj]->Data.used;
  for (obj=0; obj<manage->EnemyMax; obj++)
  {
    if (manage->enemy[obj]->Data.used == True)
    {
      DelFlag = manage->enemy[obj]->Action(&(manage->enemy[obj]->Data));
      switch (DelFlag)
      {
      case NoneDel:
        /* do nothing */
        break;
      case BossDel:
#ifdef DEBUG
        fprintf(stderr, "DelFlag == BossDel while processing Action: obj = %d\n", obj);
#endif
        if ((DelFlag == BossDel) && (manage->BossTime <= 0))
          player->Rec[0].score -= manage->enemy[obj]->Data.Point;
        manage->BossKill = True;
        /* fall off */
      case ZakoDel:
        player->Rec[0].score += manage->enemy[obj]->Data.Point;
        manage->StageShotDown++;
        /* fall off */
      case NullDel:
        DelObj(manage->enemy[obj]);
        break;
      }
    }
  }
}


static void collision_detection(void)
{
  int obj;
  int target;
  DelAtt DelFlag;
  
  for (obj=0; (obj<manage->PlayerMax); obj++)
  {
    if (manage->player[obj]->Data.used == False)
      continue;
    if (manage->player[obj]->Data.kill == True)
      continue;
    
    for (target=0; (target<manage->EnemyMax); target++)
    {
      if (manage->enemy[target]->Data.used == False)
        continue;
      if (manage->enemy[target]->Data.kill == True)
        continue;
      
      if (manage->player[obj]->Data.hitMask & manage->enemy[target]->Data.hitAtt)
      {
        if (abs(manage->player[obj]->Data.X-manage->enemy[target]->Data.X)
            > manage->player[obj]->Data.HarfW+manage->enemy[target]->Data.HarfW)
          continue;
        if (abs(manage->player[obj]->Data.Y-manage->enemy[target]->Data.Y)
            > manage->player[obj]->Data.HarfH+manage->enemy[target]->Data.HarfH)
          continue;
        
        /* crash! */
        /* we call the enemy's Hit first because the Hit of
         * the player shot 3 changes its Attatck */
        DelFlag = manage->enemy[target]->Hit(&(manage->enemy[target]->Data),&(manage->player[obj]->Data));
        switch (DelFlag)
        {
        case NoneDel:
          /* ignore it */
          break;
          
        case BossDel:
#ifdef DEBUG
          fprintf(stderr, "DelFlag == BossDel while processing Hit: target = %d, obj = %d\n", target, obj);
#endif
          if ((DelFlag == BossDel) && (manage->BossTime <= 0))
            player->Rec[0].score -= manage->enemy[target]->Data.Point;
          manage->BossKill = True;
        case ZakoDel:
          player->Rec[0].score += manage->enemy[target]->Data.Point;
          manage->StageShotDown++;
        case NullDel:
          DelObj(manage->enemy[target]);
        }
        
        if (manage->player[obj]->Hit(&(manage->player[obj]->Data),&(manage->enemy[target]->Data)) == NullDel)
          DelObj(manage->player[obj]);
      }
    }
  }
}

static int shoot_down_bonus(int percent, int loop, int stage)
{
  if (percent < 0)
  {
    fprintf(stderr, "shoot_down_bonus: negative percent given, "
            "assuming 0%%\n");
    percent = 0;
  }
  if (percent > 100)
  {
    fprintf(stderr, "shoot_down_bonus: 101+ percent given, "
            "assuming 100%%\n");
    percent = 100;
  }
  if (percent == 0)
    return 0;
  
  
  return (30000 + stage * stage * 1000) * 5 / (105 - percent);
}

/*
static int perfect_bonus(int loop, int stage)
{
  return 10000 * stage * loop;
}
*/
