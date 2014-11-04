/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: callback.c,v 1.15 2002/04/22 15:20:58 oohara Exp $ */

#include <X11/Xlib.h>
#include <X11/xpm.h>

#include "image.h"
#include "xsoldier.h"
#include "common.h"
#include "extern.h"
/* ShotToPoint */
#include "enemyshot.h"
/* sdl_draw_rect */
#include "graphic.h"

#include "callback.h"

/* action */
/* do nothing */
DelAtt NullAct(ObjData *my)
{
    return NoneDel;
}


/* hit */
/* nothing can hit me, I am immutable */
DelAtt NullHit(ObjData *my, ObjData *your)
{
    return NoneDel;
}

/* simply die */
DelAtt NullDelHit(ObjData *my, ObjData *your)
{
    return NullDel;
}

/* die with explosion */
DelAtt DeleteHit(ObjData *my, ObjData *your)
{
    NewBomb(my->X,my->Y);
    return my->EnemyAtt;
}

/* deal damage, explode if dead */
DelAtt DamageHit(ObjData *my, ObjData *your)
{
  int temp = your->Attack;
    if (my->HP < your->Attack)
      temp = my->HP;

    my->HP -= temp;
   player->Rec[0].score += temp;
    if (my->HP <= 0)
    {
      player->Rec[0].score -= 1;
	if (manage->Loop > 2)
	    ShotToPoint(my->X,my->Y,manage->player[0]->Data.X,manage->player[0]->Data.Y,5);
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

/* same above, but with big explosion */
DelAtt LargeDamageHit(ObjData *my, ObjData *your)
{
  int temp = your->Attack;
    if (my->HP < your->Attack)
      temp = my->HP;

    my->HP -= temp;
   player->Rec[0].score += temp;

    if (my->HP <= 0)
    {
      player->Rec[0].score -= 1;
	if (manage->Loop > 2)
	    ShotToPoint(my->X,my->Y,manage->player[0]->Data.X,manage->player[0]->Data.Y,5);
	NewLargeBomb(my->X,my->Y);

        my->showDamegeTime = 0;
	return my->EnemyAtt;
    }
    else
    {
      my->showDamegeTime = 15;      
      return NoneDel;
    }
    
}


/* display */
void NullReal(ObjData *my, GrpData *grp)
{
    return;
}

/* rectangle-drawing function for collision-detection debug */
void DrawRec(ObjData *my, GrpData *grp)
{
#ifdef HAVE_LIBSDL
  /* use double-standard here to keep the 10 pixel boundary */
  sdl_draw_rect(my->X-my->HarfW + 10, my->Y-my->HarfH + 10,
                my->Width, my->Height);

#else /* not HAVE_LIBSDL */
  /*
    XFillRectangle(dpy,WorkPixmap,FillGC,my->X-my->HarfW,my->Y-my->HarfH,my->Width,my->Height);
  */
    XDrawRectangle(dpy,WorkPixmap,FillGC,my->X-my->HarfW,my->Y-my->HarfH,my->Width,my->Height);
#endif /* not HAVE_LIBSDL */
    return;
}

/* display pixmap */
void DrawImage(ObjData *my, GrpData *grp)
{
    PutImage(grp->image[my->image],my->X - grp->HarfW, my->Y - grp->HarfH);

    return;
}
