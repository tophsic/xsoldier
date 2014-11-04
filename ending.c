/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: ending.c,v 1.9 2002/04/29 03:39:40 oohara Exp $ */

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

#include "xsoldier.h"
#include "extern.h"
#include "key.h"
#include "star.h"

#ifdef JSTK
#include "joystick.h"
#endif

#include "graphic.h"
#include "input.h"

#include "ending.h"

void Ending(void)
{
    char stuff[][2][32] = 
    {
    {"XSoldier staff",      ""},
    {"Producer",            "Y.Hashimoto"},
    {"Program",             "R.Masuda"},
    {"",                    "Y.Hashimoto"},
    {"Character Algorithm", "K.Inomata"},
    {"",                    "H.Yokobori"},
    {"",                    "M.Nakayama"},
    {"",                    "Y.Hashimoto"},
    {"Character Design",    "H.Hayakawa"},
    {"",                    "Y.Hashimoto"},
    {"1.x Series Developer", "Oohara Yuuma"},
    {"Special Thanks",      "N.Oohashi"},
    {"",                    "T.Yamada"},
    {"",                    "T.Igari"},
    {"",                    "RAKI all members"},
    {"",                    ".... and YOU."},
    };

    char msg[][30] = 
    {
	"Try Next Stage!!",
	"You are a great soldier!!!",
	"crazy ........"
    };

    /* number of staffs */
    int stfnumber = 16;
    int count = 0;
    int time = (stfnumber + 6) * 100;
    int len;
    int i;

    while (1)
    {
        if (waittime && (signal_delivered==0))
	    pause();
        signal_delivered = 0;

	if (count < time)
        {
            count++;
        }

        if (event_handle_ending() == 0)
          return;

        clear_window();
	DrawStar(StarPtn1);
	DrawStar(StarPtn2);

	for (i=0; i<stfnumber; i++)
	{
	    if (i == 0)
	    {
		draw_string(210, FieldH - count + 100 * i,
                            stuff[i][0], strlen(stuff[i][0]));
		draw_string(450, FieldH - count + 100 * i,
                            stuff[i][1], strlen(stuff[i][1]));
	    }
	    else
	    {
		draw_string(100, FieldH - count + 100 * i,
                            stuff[i][0], strlen(stuff[i][0]));
		draw_string(300, FieldH - count + 100 * i + 30,
                            stuff[i][1], strlen(stuff[i][1]));
	    }
	}
	
	if (count > time-30)
	{
	    len = count - (time-30);

	    if (manage->Loop < 3)
	    {
		if (len >= strlen(msg[manage->Loop-1]))
                {
                  draw_string(200, 440, "Press space key",
                              strlen("Press space key"));
                  len = strlen(msg[manage->Loop-1]);
                }
                
		draw_string(200, 400, msg[manage->Loop-1], len);
	    }
	    else
	    {
		if (len >= strlen(msg[2]))
                {
                  draw_string(200, 440, "Press space key",
                              strlen("Press space key"));
                  len = strlen(msg[2]);
                }
	    }
	}

        redraw_window();
    }
}
