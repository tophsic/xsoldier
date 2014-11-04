/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: common.c,v 1.4 2002/04/29 03:38:41 oohara Exp $ */

/*
#include <X11/Xlib.h>
#include <X11/xpm.h>
*/
/* abs */
#include <stdlib.h>

#include "image.h"
#include "xsoldier.h"
#include "common.h"
#include "callback.h"
#include "extern.h"
#include "sin.h"

void NewBomb(int x, int y)
{
    int i;

    if (manage->EnemyNum >= manage->EnemyMax)
        return;

    for (i=1; i<manage->EnemyMax; i++)
    {
        if (manage->enemy[i]->Data.used == False)
	{
	    manage->Bomb.Data.X = x;
	    manage->Bomb.Data.Y = y;

	    manage->enemy[i]->Data    = manage->Bomb.Data;
	    manage->enemy[i]->Grp     = manage->Bomb.Grp;
            manage->enemy[i]->Action  = BombAct;
            manage->enemy[i]->Realize = DrawImage;
            manage->enemy[i]->Hit     = NullHit;

	    manage->EnemyNum++;
	    return;
	}
    }
}

void NewLargeBomb(int x, int y)
{
    int i;

    if (manage->EnemyNum >= manage->EnemyMax)
        return;

    for (i=1; i<manage->EnemyMax; i++)
    {
        if (manage->enemy[i]->Data.used == False)
	{
	    manage->LargeBomb.Data.X = x;
	    manage->LargeBomb.Data.Y = y;

	    manage->enemy[i]->Data    = manage->LargeBomb.Data;
	    manage->enemy[i]->Grp     = manage->LargeBomb.Grp;
            manage->enemy[i]->Action  = BombAct;
            manage->enemy[i]->Realize = DrawImage;
            manage->enemy[i]->Hit     = NullHit;

	    manage->EnemyNum++;
	    return;
	}
    }
}

DelAtt BombAct(ObjData *my)
{
    my->image = my->Cnt[0];
    my->Cnt[0]++;
    if (my->Cnt[0] > 5)
	return NullDel;
    return NoneDel;
}

int GetDirection(int mx, int my, int sx, int sy)
{
    static double hi;
    static int uw;
    static int uh;
    static int h;
    static int w;

    uw = abs(sx-mx);
    uh = abs(sy-my);
    h = sy-my;
    w = sx-mx;

    if (!uw) return (uh>0)?4:0;
    if (!uh) return (uw>0)?2:6;

    hi = (double)uh/uw;
    if (hi < 0.42)
        return (w > 0) ? 2: 6;
    else if (hi > 2.42)
        return (h > 0) ? 4: 0;
    else
    {
        return (w>0)?((h>0)?3:1):((h>0)?5:7);
        /***
        if (w > 0 && h > 0) return 3;
        if (w > 0 && h < 0) return 1;
        if (w < 0 && h > 0) return 5;
        if (w < 0 && h < 0) return 7;
        ***/
    }
}
