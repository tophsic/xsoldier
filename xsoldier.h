/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: xsoldier.h,v 1.12 2009/11/08 05:47:15 oohara Exp $ */

/* HAVE_RAND */
#include <config.h>

#if !defined _XSOLDIER_H_
#define _XSOLDIER_H_

#include <stdio.h>
#include "image.h"
/*
#include <X11/Xlib.h>
#include <X11/Xutil.h>
*/

#if ! defined(_XtIntrinsic_h) && ! defined(PIXEL_ALREADY_TYPEDEFED)
typedef unsigned long Pixel;
#define PIXEL_ALREADY_TYPEDEFED
#endif

#ifndef HAVE_RAND
# define srand(x) srand48((long)x)
# define rand() lrand48()
#endif

#ifndef WAIT
#define WAIT 35000
#endif /* WAIT */

#define PIXMAP DATADIR "/games/xsoldier/"
#define SCORE LOCALSTATEDIR "/games/xsoldier/"
#define SCOREFILE "xsoldier.scores"

#define MaxStage 8
#define ShotTiming 100
#define MaxLevel 80

#define FIRST1UP 200000
#define EVERY1UP 200000

/* attribute mask */
#define MPlayer (1L<<0)
#define MPShot  (1L<<1)
#define MEnemy  (1L<<2)
#define MEShot  (1L<<3)
#define MItem   (1L<<4)

#ifndef True
#define True 1
#endif /* True */
#ifndef False
#define False 0
#endif /* FALSE */

/* death flag */
typedef enum {NoneDel,NullDel,ZakoDel,BossDel} DelAtt;

/* basic data of object */
typedef struct
{
    int used;

    int hitAtt;
    int hitMask;
    int Width,Height;
    int HarfW,HarfH;
    /* don't modify above */

  /* shoot if shotTime >= ShotTiming */
    int startTime;
    int shotTime;

  int kill;
    int HP;
    int Attack;
    int Point;
    DelAtt EnemyAtt;

    int X,Y;
    int oldX,oldY;
    int inertX,inertY;
    int Angle;
    int Speed;
    int Cnt[16];

    int image;
  int showDamegeTime;
  int notShootingTime;
  int shouldAct;
} ObjData;

/* graphic data of object */
typedef struct
{
    Image **image;

    int Width,Height;
    int HarfW,HarfH;
} GrpData;

/* prototype of object data */
typedef struct
{
    ObjData Data;
    GrpData Grp;

    DelAtt (*Action)(ObjData *my);
    DelAtt (*Hit)(ObjData *my, ObjData *your);
    void (*Realize)(ObjData *my, GrpData *grp);
} CharObj;

/* table of objects */
typedef struct
{
    CharObj **player;
    CharObj **enemy;

    CharObj New;

    int PlayerMax;
    int PlayerNum;
    int EnemyMax;
    int EnemyNum;

  /* frequently used objects */
    CharObj EnemyShot;
    CharObj Bomb;
    CharObj LargeBomb;

    int Level;
    int Stage;
    int Loop;
    int Appear;

    int StageEnemy;
    int StageShotDown;
    int ZakoApp;
    int BossApp;
    int BossKill;

  int BossTime;
  int flag_maxlevel;
  int start_power;
  int showShootDown;
  int flag_nopausemessage;
  int program_should_quit;
} CharManage;
/* this is global because we have only one table */

typedef struct
{
    char name[16];
    int score;
    int stage;
    int loop;
} Record;

typedef struct
{
    Record Rec[11];
    int Ships;
    int Percent;
    int Next;
} PlayerData;

#endif
