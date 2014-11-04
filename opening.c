/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: opening.c,v 1.10 2002/04/29 03:42:28 oohara Exp $ */

/* JSTK is defined in config.h */
#include <config.h>
/* pause */
#include <unistd.h>
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
#include "extern.h"
#include "key.h"
#include "star.h"
#include "version.h"

#include "graphic.h"
#include "input.h"

#include "opening.h"

int Opening(void)
{
    Image **Title;
    int count = 0;
    int status;

    ChangeStarParameter(StarPtn1,5);
    ChangeStarParameter(StarPtn2,10);

    Title = ImageInit(PIXMAP "/Title.xpm" GZ,1);

    while(1)
    {
        if (waittime && (signal_delivered==0))
	    pause();
        signal_delivered = 0;

	if (count < 500)
	    count++;
	else
	    count = 0;

        status = event_handle_opening();
        if (status != 0)
        {
          FreeImages(Title,1);
          return status;
        }

        clear_window();
        
        /*
	XFillRectangle(dpy,WorkPixmap,BackGC,0,0,FieldW,FieldH);
        */
	DrawStar(StarPtn1);
	DrawStar(StarPtn2);

	PutImage(Title[0],50,50);
	draw_string(410, 210, "Ver. " VERSION, strlen("Ver. " VERSION));
	draw_string(410, 225, UPDATE, strlen(UPDATE));

	if (count%30 < 15)
	{
	    draw_string(170, 630, "Press [SPACE KEY] to start",
                        strlen("Press [SPACE KEY] to start"));
#ifdef JSTK
	    draw_string(155, 610, "Press [Joystick BUTTON] to start",
                        strlen("Press [Joystick BUTTON] to start"));
#endif
	}

	if (count < 250)
	{
	    char buff[64];
	    int i;
	    draw_string(210, 270, "Top 10 Soldiers",
                        strlen("Top 10 Soldiers"));
	    draw_string(120, 320, "Name", strlen("Name"));
	    draw_string(270, 320, "Stage", strlen("Stage"));
            draw_string(350, 320, "Score", strlen("Score"));
	    for (i=1; i<=10; i++)
	    {
		draw_string(120, 330+i*25, player->Rec[i].name,
                            strlen(player->Rec[i].name));
		sprintf(buff,"%2d-%2d",player->Rec[i].loop,
                        player->Rec[i].stage);
		draw_string(270, 330+i*25, buff, strlen(buff));
		sprintf(buff,"%8d",player->Rec[i].score);
		draw_string(350, 330+i*25, buff, strlen(buff));
	    }
	}
	else
	{
	    draw_string(220, 270, "How To Play", strlen("How To Play"));
	    draw_string(180, 330, "Up", strlen("Up"));
	    draw_string(260, 330, " .... ", strlen(" .... "));
	    draw_string(300, 330, upKey, strlen(upKey));
            draw_string(180, 355, "Down", strlen("Down"));
	    draw_string(260, 355, " .... ", strlen(" .... "));
	    draw_string(300, 355, downKey, strlen(downKey));
	    draw_string(180, 380, "Left", strlen("Left"));
	    draw_string(260, 380, " .... ", strlen(" .... "));
	    draw_string(300, 380, leftKey, strlen(leftKey));
            draw_string(180, 405, "Right",strlen("Right"));
	    draw_string(260, 405, " .... ", strlen(" .... "));
	    draw_string(300, 405, rightKey, strlen(rightKey));
	    draw_string(180, 430, "Shot", strlen("Shot"));
	    draw_string(260, 430, " .... ", strlen(" .... "));
	    draw_string(300, 430, shotKey, strlen(shotKey));
	    draw_string(180, 455, "Speed up", strlen("Speed up"));
	    draw_string(260, 455, " .... ",strlen(" .... "));
	    draw_string(300, 455, spdupKey, strlen(spdupKey));
	    draw_string(180, 480, "Speed down", strlen("Speed down"));
	    draw_string(260, 480, " .... ", strlen(" .... "));
	    draw_string(300, 480, spdwnKey, strlen(spdwnKey));
	    draw_string(180, 505, "Pause", strlen("Pause"));
	    draw_string(260, 505, " .... ", strlen(" .... "));
	    draw_string(300, 505, pauseKey, strlen(pauseKey));
	    draw_string(180, 530, "Quit", strlen("Quit"));
	    draw_string(260, 530, " .... ", strlen(" .... "));
	    draw_string(300, 530, quitKey, strlen(quitKey));
#ifdef DEBUG
	    draw_string(220, 560, "Debug-only Commands",
                        strlen("Debug-only Commands"));
	    draw_string(180, 575, "Change Weapon", strlen("Change Weapon"));
	    draw_string(300, 575, weaponchangeKey,
                        strlen(weaponchangeKey));
	    draw_string(180, 590, "Clear Enemy Shots",
                        strlen("Clear Enemy Shots"));
	    draw_string(300, 590, clearenemyshotKey,
                        strlen(clearenemyshotKey));
#endif
	}
        redraw_window();
    }
}
