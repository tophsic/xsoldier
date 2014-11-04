/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: joystick.c,v 1.6 2011/08/10 14:25:38 oohara Exp $ */

#include <config.h>

#ifdef JSTK

/* read */
#include <unistd.h>
/* exit */
#include <stdlib.h>

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h> /*** SystemV ioctl() ***/
#include <linux/joystick.h>

#include <X11/Xlib.h>

#include "image.h"
#include "xsoldier.h"
#include "key.h"
#include "extern.h"

#endif /* JSTK */

#include "joystick.h"

#ifdef JSTK

static int fd;

#endif /* JSTK */

void initJS(void)
{
#ifdef JSTK

    long tmpl;

    if ((fd=open("/dev/js0",O_RDONLY)) == -1)
    {
	perror("/dev/js0");
	if ((fd=open("/dev/js1",O_RDONLY)) == -1)
	{
	    perror("/dev/js1");
	    exit(1);
	}
    }

    tmpl = 1;
    if ((ioctl(fd,JS_SET_TIMELIMIT,&tmpl)) == -1)
    {
        perror ("js_set");
        exit (1);
    }

#endif /* JSTK */
}

void readJS(void)
{
#ifdef JSTK

    static struct JS_DATA_TYPE js;
    static int scrsv = 0;
    /* if the button 2 was pressed? */ 
   static int psd2btn = False;

    if (scrsv > 200)
    {
#ifndef HAVE_LIBSDL
	XResetScreenSaver(dpy);
#endif /* not HAVE_LIBSDL */
	scrsv = 0;
    }
    else
	scrsv++;

    joymask = 0;
    if ((read(fd,&js,JS_RETURN)) == JS_RETURN)
    {
	if (js.buttons & 1) joymask |= Shot;

	if (js.buttons & 2)
	{
	    if (psd2btn == False)
	    {
		joymask |= SpeedUP;
		psd2btn = True;
	    }
	}
	else
	    psd2btn = False;
        /* you have to release the button 2 to change the speed again */

        /* applied fix available at
         * http://www.surfline.ne.jp/hachi/xsoldier.html
         */
	if (js.x <  100) joymask |= Left;
	if (js.x >  150) joymask |= Right; /* Was 1000 */
	if (js.y <  100) joymask |= Up;
	if (js.y >  150) joymask |= Down; /* Was 1000 */
    }

#endif /* JSTK */
}
