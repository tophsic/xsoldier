/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: input.c,v 1.17 2006/09/16 09:20:59 oohara Exp $ */

#include <config.h>

#ifdef HAVE_LIBSDL
#include <SDL.h>
#else /* HAVE_LIBSDL */
#include <X11/Xlib.h>
#endif  /* HAVE_LIBSDL */

/*
#include <X11/Xutil.h>
*/

#include "image.h"
#include "xsoldier.h"
#include "extern.h"
#include "graphic.h"
#include "input.h"
#include "key.h"
#include "manage.h"

#ifdef JSTK
#include "joystick.h"
#endif /* JSTK */


static void SetKeyMask(KeySym key);
static void UnsetKeyMask(KeySym key);

int input_init(void)
{
  keymask = 0;
#ifdef JSTK
  /* initialize joystick */
  initJS();
#endif /* JSTK */

#ifdef HAVE_LIBSDL
  upKey    = SDL_GetKeyName(SDLK_UP);
  downKey  = SDL_GetKeyName(SDLK_DOWN);
  leftKey  = SDL_GetKeyName(SDLK_LEFT);
  rightKey = SDL_GetKeyName(SDLK_RIGHT);
  shotKey  = SDL_GetKeyName(SDLK_LSHIFT);
  spdupKey = SDL_GetKeyName(SDLK_a);
  spdwnKey = SDL_GetKeyName(SDLK_s);
  pauseKey = SDL_GetKeyName(SDLK_p);
  quitKey  = SDL_GetKeyName(SDLK_q);
#ifdef DEBUG
  weaponchangeKey = SDL_GetKeyName(SDLK_w);
  clearenemyshotKey = SDL_GetKeyName(SDLK_c);
#endif /* DEBUG */

#else /* not HAVE_LIBSDL */
  upKey    = XKeysymToString(UpKey);
  downKey  = XKeysymToString(DownKey);
  leftKey  = XKeysymToString(LeftKey);
  rightKey = XKeysymToString(RightKey);
  shotKey  = XKeysymToString(ShotKey);
  spdupKey = XKeysymToString(SpeedUPKey);
  spdwnKey = XKeysymToString(SpeedDOWNKey);
  pauseKey = XKeysymToString(PauseKey);
  quitKey  = XKeysymToString(QuitKey);
#ifdef DEBUG
  weaponchangeKey = XKeysymToString(WeaponChangeKey);
  clearenemyshotKey = XKeysymToString(ClearEnemyShotKey);
#endif /* DEBUG */

#endif /* not HAVE_LIBSDL */
  return 0;  
}


/* return 0 if the game is over, 1 if not */
int event_handle(void)
{
#ifdef HAVE_LIBSDL
  while (SDL_PollEvent(&event) > 0)
  {
    if (event.type == SDL_QUIT)
    { 
      manage->program_should_quit = True;
      return 0;
    }
    else if (event.type == SDL_VIDEOEXPOSE)
    { 
      redraw_window();
    }
    else if (event.type == SDL_ACTIVEEVENT)
    {
      if (event.active.gain == 1)
        /* the mouse cursor entered the window */
        ;
      else
      {
        if (manage->player[0]->Data.used==False && player->Ships==0)
          return 0;
        else
        {
          keymask = 0;
          keymask |= Pause;
        }
      }
    }
    else if (event.type == SDL_KEYDOWN)
    {
      if (manage->player[0]->Data.used==False && player->Ships==0)
        /* key press after the game is over */
        return 0;
      else
        SetKeyMask(event.key.keysym.sym);

      if (keymask&Quit)
        return 0;
    }
    else if (event.type == SDL_KEYUP)
      UnsetKeyMask(event.key.keysym.sym);
    
  }
  

#else /* not HAVE_LIBSDL */
  while(XPending(dpy))
  {
    XNextEvent(dpy,&event);
    switch(event.type)
    {
    case Expose:
      if (event.xexpose.count == 0)
        XCopyArea(dpy,WorkPixmap,win,BackGC,0,0,FieldW,FieldH,0,0);
      break;
    case EnterNotify:
      XAutoRepeatOff(dpy);
      break;
    case LeaveNotify:
      XAutoRepeatOn(dpy);
      if (manage->player[0]->Data.used==False && player->Ships==0)
        return 0;
      else
      {
        keymask = 0;
        keymask |= Pause;
      }
      break;
    case KeyPress:
      if (manage->player[0]->Data.used==False && player->Ships==0)
        /* key press after the game is over */
        return 0;
      else
        SetKeyMask(XLookupKeysym(&(event.xkey),0));
      
      if (keymask&Quit)
        return 0;
      break;
    case KeyRelease:
      UnsetKeyMask(XLookupKeysym(&(event.xkey),0));
      break;
    }
  }
#endif /* not HAVE_LIBSDL */


#ifdef JSTK
  readJS();
  if ((manage->player[0]->Data.used==False && player->Ships==0) && (joymask&Shot || joymask&SpeedUP))
    /* key press after the game is over */
    return 0; 
#endif
  return 1;
}

/* return
 * 1 if the game starts
 * -1 if the program quits
 * 0 otherwise
 */
int event_handle_opening(void)
{
#ifdef HAVE_LIBSDL
  while (SDL_PollEvent(&event) > 0)
  {
    if (event.type == SDL_QUIT)
    { 
      manage->program_should_quit = True;
      return -1;
    }
    else if (event.type == SDL_VIDEOEXPOSE)
    { 
      ;
    }
    else if (event.type == SDL_ACTIVEEVENT)
    {
      if (event.active.gain == 1)
        /* the mouse cursor entered the window */
        ;
      else
        ;
    }
    else if (event.type == SDL_KEYDOWN)
    {
      if (event.key.keysym.sym == SDLK_SPACE)
        return 1;
      if (event.key.keysym.sym == SDLK_q)
        return -1;
    }
  }

#else /* not HAVE_LIBSDL */
  while(XPending(dpy))
  {
    XNextEvent(dpy,&event);
    switch(event.type)
    {
    case Expose:
      break;
    case EnterNotify:
      XAutoRepeatOff(dpy);
      break;
    case LeaveNotify:
      XAutoRepeatOn(dpy);
      break;
    case KeyPress:
      if (XLookupKeysym(&(event.xkey),0) == XK_space)
      {
        return 1;
      }
      if (XLookupKeysym(&(event.xkey),0) == QuitKey)
      {
        return -1;
      }
      break;
    }
  }

#endif /* not HAVE_LIBSDL */

#ifdef JSTK
  readJS();
  if ((joymask&Shot) || (joymask&SpeedUP))	
  {
    return 1;
  }
#endif
  return 0;
}

/* return 0 if the ending is over, 1 if not */
int event_handle_ending(void)
{
#ifdef HAVE_LIBSDL
  while (SDL_PollEvent(&event) > 0)
  {
    if (event.type == SDL_QUIT)
    {
      manage->program_should_quit = True;
      return 0;
    }
    else if (event.type == SDL_VIDEOEXPOSE)
    { 
      ;
    }
    else if (event.type == SDL_ACTIVEEVENT)
    {
      if (event.active.gain == 1)
        /* the mouse cursor entered the window */
        ;
      else
        ;
    }
    else if (event.type == SDL_KEYDOWN)
    {
      if (event.key.keysym.sym == SDLK_SPACE)
        return 0;
    }
  }

#else /* not HAVE_LIBSDL */

  while(XPending(dpy))
  {
    XNextEvent(dpy,&event);
    switch(event.type)
    {
    case EnterNotify:
      XAutoRepeatOff(dpy);
      break;
    case LeaveNotify:
      XAutoRepeatOn(dpy);
      break;
    case KeyPress:
      if (XLookupKeysym(&(event.xkey),0) == XK_space)
      {
          return 0;
      }
    }
  }

#endif /* not HAVE_LIBSDL */

	
#ifdef JSTK
  readJS();
  if ((joymask&Shot) || (joymask&SpeedUP))	
  {
      return 0;
  }
#endif
  return 1;
}

static void SetKeyMask(KeySym key)
{
  if (key ==  UpKey)
    keymask |= Up;
  else if (key == DownKey)
    keymask |= Down;
  else if (key == LeftKey)
    keymask |= Left;
  else if (key == RightKey)
    keymask |= Right;
  else if (key == ShotKey)
    keymask |= Shot;
  else if (key == SpeedUPKey)
    keymask |= SpeedUP;
  else if (key == SpeedDOWNKey)
    keymask |= SpeedDOWN;
  else if (key == PauseKey)
    /* toggle */
    keymask ^= Pause;
  else if (key == QuitKey)
    keymask |= Quit;
#ifdef DEBUG
  else if (key == XK_w)
  {
    if (manage->player[0]->Data.Cnt[5] == 3)
      manage->player[0]->Data.Cnt[5] = 0;
    manage->player[0]->Data.Cnt[5]++;
  }
  else if (key == XK_c)
    ClearEnemyShotManage(manage);
#endif
}

static void UnsetKeyMask(KeySym key)
{
  if (key ==  UpKey)
    keymask &= ~Up;
  else if (key == DownKey)
    keymask &= ~Down;
  else if (key == LeftKey)
    keymask &= ~Left;
  else if (key == RightKey)
    keymask &= ~Right;
  else if (key == ShotKey)
    keymask &= ~Shot;
  else if (key == SpeedUPKey)
    keymask &= ~SpeedUP;
  else if (key == SpeedDOWNKey)
    keymask &= ~SpeedDOWN;
  else if (key == QuitKey)
    keymask &= ~Quit;
}
