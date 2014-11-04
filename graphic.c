/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: graphic.c,v 1.35 2002/05/06 04:28:44 oohara Exp $ */
#include <config.h>

#include <stdio.h>
/* exit, atexit */
#include <stdlib.h>
/* isprint */
#include <ctype.h>

#ifdef HAVE_LIBSDL
#include <SDL.h>
#else /* not HAVE_LIBSDL */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/xpm.h>
#endif /* not HAVE_LIBSDL */

/* Image */
#include "image.h"
#include "graphic.h"
#include "xsoldier.h"
#include "star.h"

#include "extern.h"
#if 0
static GC FontGC;
static GC BackGC;
static GC FillGC;
static XColor black;
static XColor white;

static Display *dpy;
static Colormap cmap;
static Window root;
static Window win;
static Pixmap WorkPixmap;
#endif /* 0 */

static Image **Font1Image;
static Image **Font2Image;
static Image **Font3Image;
static Image **Font4Image;
static Image **Font5Image;
static Image **Font6Image;

#ifdef HAVE_LIBSDL
static int sdl_draw_pixel(int x, int y);
#endif /* HAVE_LIBSDL */

int graphic_init(void)
{
#ifdef HAVE_LIBSDL
  SDL_Rect temp;
#else /* not HAVE_LIBSDL */
  XColor blackTrue;
  XColor whiteTrue;
  XSizeHints sh;
#endif /* not HAVE_LIBSDL */
  FieldW  = 500;
  FieldH  = 650;

#ifdef HAVE_LIBSDL
  if(SDL_Init(SDL_INIT_VIDEO) < 0 )
  {
    fprintf(stderr, "cannot initialize SDL (%s)\n", SDL_GetError());
    exit(1);
  }
  if (atexit(SDL_Quit) != 0)
  {
    fprintf(stderr, "cannot register SDL_Quit to exit\n");
    SDL_Quit();
    exit(1);
  }

  /*
  dpy = SDL_SetVideoMode(FieldW + 20, FieldH + 20, 0, SDL_FULLSCREEN);
  */
  dpy = SDL_SetVideoMode(FieldW + 20, FieldH + 20, 0, 0);
  if (dpy == NULL)
  {
    fprintf(stderr, "SDL_SetVideoMode failed (%s)\n", SDL_GetError());
    exit(1);
  }

  SDL_WM_SetCaption("xsoldier", "xsoldier");
  /*
  SDL_ShowCursor(SDL_DISABLE);
  */
  sdl_draw_rect(6, 6, FieldW + 5, FieldH + 5);
  
  temp.x = 10;
  temp.y = 10;
  temp.w = FieldW;
  temp.h = FieldH;
  SDL_SetClipRect(dpy, &temp);

#else /* not HAVE_LIBSDL */

  dpy = XOpenDisplay(display);
  if (dpy == NULL)
  {
    fprintf(stderr,"graphic_init: [%s] can't open display.\n", display);
    exit(1);
  }

  cmap = XCreateColormap(dpy, RootWindow(dpy, 0), DefaultVisual(dpy, 0),
                         AllocNone);

  XAllocNamedColor(dpy, cmap, "black", &black, &blackTrue);
  XAllocNamedColor(dpy, cmap, "white", &white, &whiteTrue);

  root = XCreateSimpleWindow(dpy, RootWindow(dpy,0), 0, 0, FieldW+20,
                             FieldH+20, 0, white.pixel, black.pixel);

  XSetWindowColormap(dpy, root, cmap);

  XSelectInput(dpy, root, ExposureMask|EnterWindowMask|LeaveWindowMask|KeyPressMask|KeyReleaseMask);
  XStoreName(dpy, root, "xsoldier");
  sh.flags = (PMaxSize | PMinSize);
  sh.min_width = FieldW + 20;
  sh.min_height = FieldH + 20;
  sh.max_width = FieldW + 20;
  sh.max_height = FieldH + 20;
  XSetWMNormalHints(dpy, root, &sh);

  win = XCreateSimpleWindow(dpy, root, 10, 10, FieldW, FieldH, 1,
                            white.pixel, black.pixel);
  XSelectInput(dpy, win, ExposureMask|EnterWindowMask|KeyPressMask|KeyReleaseMask);

  WorkPixmap = XCreatePixmap(dpy, win, FieldW, FieldH, DefaultDepth(dpy, 0));
  /*
  font       = XLoadFont(dpy,"-adobe-helvetica-bold-r-normal--12-120-75-75-p-70-iso8859-1");
  */
  FontGC     = XCreateGC(dpy,root,0,0);
  XSetGraphicsExposures(dpy,FontGC,False);
  /*
  XSetForeground(dpy,FontGC,white.pixel);
  XSetFont(dpy,FontGC,font);
  */

  BackGC       = XCreateGC(dpy,WorkPixmap,0,0);
  XSetGraphicsExposures(dpy,BackGC,False);
  XSetForeground(dpy,BackGC,black.pixel);

  FillGC       = XCreateGC(dpy,WorkPixmap,0,0);
  XSetGraphicsExposures(dpy,FillGC,False);
  XSetForeground(dpy,FillGC,white.pixel);

#endif  /* not HAVE_LIBSDL */

  PlayerImage = ImageInit(PIXMAP "/Player.xpm" GZ,6);
  PShot1Image  = ImageInit(PIXMAP "/PlayerShot1.xpm" GZ,2);
  PShot2Image  = ImageInit(PIXMAP "/PlayerShot2.xpm" GZ,2);
  PShot3Image  = ImageInit(PIXMAP "/PlayerShot3.xpm" GZ,3);

  EShotImage  = ImageInit(PIXMAP "/EnemyShot.xpm" GZ,4);
  ELaserImage  = ImageInit(PIXMAP "/EnemyLaser.xpm" GZ,1);
  EMissileImage  = ImageInit(PIXMAP "/EnemyMiss.xpm" GZ,8);
  EBoundImage = ImageInit(PIXMAP "/EnemyBound.xpm" GZ,8);
  ERingImage = ImageInit(PIXMAP "/EnemyRing.xpm" GZ,4);

  BombImage   = ImageInit(PIXMAP "/ExpSmall.xpm" GZ,5);
  LargeBombImage= ImageInit(PIXMAP "/ExpLarge.xpm" GZ,5);

  Enemy1Image = ImageInit(PIXMAP "/Enemy1.xpm" GZ,8);
  Enemy2Image = ImageInit(PIXMAP "/Enemy2.xpm" GZ,8);
  Enemy3Image = ImageInit(PIXMAP "/Enemy3.xpm" GZ,8);
  Enemy4Image = ImageInit(PIXMAP "/Enemy4.xpm" GZ,8);
  Enemy5Image = ImageInit(PIXMAP "/Enemy5.xpm" GZ,4);
  Enemy6Image = ImageInit(PIXMAP "/Enemy6.xpm" GZ,6);
  Enemy7Image = ImageInit(PIXMAP "/Enemy7.xpm" GZ,1);

  Boss1Image = ImageInit(PIXMAP "/Boss1.xpm" GZ,1);
  Boss2Image = ImageInit(PIXMAP "/Boss2.xpm" GZ,1);
  Boss3Image = ImageInit(PIXMAP "/Boss3.xpm" GZ,1);
  Boss4Image = ImageInit(PIXMAP "/Boss4.xpm" GZ,1);
  Boss5Image = ImageInit(PIXMAP "/Boss5.xpm" GZ,1);
  Boss6Image = ImageInit(PIXMAP "/Boss6.xpm" GZ,2);
  Boss7Image = ImageInit(PIXMAP "/Boss7.xpm" GZ,1);

  ItemImage = ImageInit(PIXMAP "/Item.xpm" GZ,4);
    
  /* initialize stars */
  InitStarModule(FieldW,FieldH);
  StarPtn1 = CreateStar(PIXMAP "/Star1.xpm" GZ,4,5,5);
  StarPtn2 = CreateStar(PIXMAP "/Star2.xpm" GZ,4,10,10);

  /* initialize font */
  /* explanation of font images
   *  14 * 7
   *  0@P`p 14
   * !1AQaq 28
   * "2BRbr 42
   * #3CScs 56
   * $4DTdt 70
   * %5EUeu 84
   * &6FVfv 98
   * '7GWgw 112
   * (8HXhx 126
   * )9IYiy 140
   * *:JZjz 154
   * +;K[k{ 168
   * ,<L\l| 182
   * -=M]m} 196
   * .>N^n~ 210
   * /?O_o  224
   */
  Font1Image = ImageInit(PIXMAP "/font1.xpm" , 16);
  Font2Image = ImageInit(PIXMAP "/font2.xpm" , 16);
  Font3Image = ImageInit(PIXMAP "/font3.xpm" , 16);
  Font4Image = ImageInit(PIXMAP "/font4.xpm" , 16);
  Font5Image = ImageInit(PIXMAP "/font5.xpm" , 16);
  Font6Image = ImageInit(PIXMAP "/font6.xpm" , 16);

  return 0;
}

int clear_window(void)
{
#ifdef HAVE_LIBSDL
  if (SDL_FillRect(dpy, NULL, SDL_MapRGB(dpy->format, 0, 0, 0)) != 0)
  {
    fprintf(stderr, "clear_window: SDL_FillRect failed (%s)\n",
            SDL_GetError());
    exit(1);
  }
  
#else /* not HAVE_LIBSDL */
  XFillRectangle(dpy, WorkPixmap, BackGC, 0, 0, FieldW, FieldH);
#endif /* not HAVE_LIBSDL */
  return 0;
}


int redraw_window(void)
{
#ifdef HAVE_LIBSDL
  if (SDL_Flip(dpy) != 0)
  {
    fprintf(stderr, "redraw_window: SDL_Flip failed (%s)\n",
            SDL_GetError());
    exit(1);
  }
  
#else /* not HAVE_LIBSDL */
  XCopyArea(dpy,WorkPixmap,win,BackGC,0,0,FieldW,FieldH,0,0);
  XFlush(dpy);
  XSync(dpy,False);
#endif /* not HAVE_LIBSDL */
  return 0;
}

int graphic_finish(void)
{
  FreeImages(PlayerImage,6);
  FreeImages(PShot1Image,2);
  FreeImages(PShot2Image,2);
  FreeImages(PShot3Image,3);

  FreeImages(EShotImage,4);
  FreeImages(ELaserImage,1);
  FreeImages(EMissileImage,8);
  FreeImages(EBoundImage,8);
  FreeImages(ERingImage,4);

  FreeImages(BombImage,5);
  FreeImages(LargeBombImage,5);

  FreeImages(Enemy1Image,8);
  FreeImages(Enemy2Image,8);
  FreeImages(Enemy3Image,8);
  FreeImages(Enemy4Image,8);
  FreeImages(Enemy5Image,4);
  FreeImages(Enemy6Image,6);
  FreeImages(Enemy7Image,1);

  FreeImages(Boss1Image,1);
  FreeImages(Boss2Image,1);
  FreeImages(Boss3Image,1);
  FreeImages(Boss4Image,1);
  FreeImages(Boss5Image,1);
  FreeImages(Boss6Image,2);
  FreeImages(Boss7Image,1);

  FreeImages(ItemImage,4);

  FreeImages(Font1Image, 16);
  FreeImages(Font2Image, 16);
  FreeImages(Font3Image, 16);
  FreeImages(Font4Image, 16);
  FreeImages(Font5Image, 16);
  FreeImages(Font6Image, 16);

  DeleteAllStar();
#ifdef HAVE_LIBSDL
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
#else /* not HAVE_LIBSDL */
  XFreeGC(dpy, FontGC);
  XFreeGC(dpy, BackGC);
  XFreeGC(dpy, FillGC);
  
  XAutoRepeatOn(dpy);
  XFlush(dpy);
  XCloseDisplay(dpy);
#endif /* not HAVE_LIBSDL */
  return 0;
}


int draw_string(int x, int y, const char *string, int length)
{
  int i;
  /*
  XDrawString(dpy, WorkPixmap, FontGC, x, y, string, length);
  */
  /* XDrawString seens to have its own origin */
  y -= 7;
  for (i = 0; (i < length) && (string[i] != '\0'); i++)
  {
    draw_char(x, y, string[i]);
    x += 7;
  }
  
  return 0;
}

/* return 0 on success, negative value on error */
int draw_char(int x, int y, int c)
{
  if (!isprint(c))
  {
    fprintf(stderr, "draw_char: unprintable char found (\\x%x), "
            "replacing it with ?\n", c);
    c = '?';
  }

  /* don't assume ASCII */
  switch (c)
  {
  case ' ':
    /* do nothing */
    return 0;
  case '!':
    PutImage(Font1Image[1], x, y);
    return 0;
  case '"':
    PutImage(Font1Image[2], x, y);
    return 0;
  case '#':
    PutImage(Font1Image[3], x, y);
    return 0;
  case '$':
    PutImage(Font1Image[4], x, y);
    return 0;
  case '%':
    PutImage(Font1Image[5], x, y);
    return 0;
  case '&':
    PutImage(Font1Image[6], x, y);
    return 0;
  case '\'':
    PutImage(Font1Image[7], x, y);
    return 0;
  case '(':
    PutImage(Font1Image[8], x, y);
    return 0;
  case ')':
    PutImage(Font1Image[9], x, y);
    return 0;
  case '*':
    PutImage(Font1Image[10], x, y);
    return 0;
  case '+':
    PutImage(Font1Image[11], x, y);
    return 0;
  case ',':
    PutImage(Font1Image[12], x, y);
    return 0;
  case '-':
    PutImage(Font1Image[13], x, y);
    return 0;
  case '.':
    PutImage(Font1Image[14], x, y);
    return 0;
  case '/':
    PutImage(Font1Image[15], x, y);
    return 0;
  case '0':
    PutImage(Font2Image[0], x, y);
    return 0;
  case '1':
    PutImage(Font2Image[1], x, y);
    return 0;
  case '2':
    PutImage(Font2Image[2], x, y);
    return 0;
  case '3':
    PutImage(Font2Image[3], x, y);
    return 0;
  case '4':
    PutImage(Font2Image[4], x, y);
    return 0;
  case '5':
    PutImage(Font2Image[5], x, y);
    return 0;
  case '6':
    PutImage(Font2Image[6], x, y);
    return 0;
  case '7':
    PutImage(Font2Image[7], x, y);
    return 0;
  case '8':
    PutImage(Font2Image[8], x, y);
    return 0;
  case '9':
    PutImage(Font2Image[9], x, y);
    return 0;
  case ':':
    PutImage(Font2Image[10], x, y);
    return 0;
  case ';':
    PutImage(Font2Image[11], x, y);
    return 0;
  case '<':
    PutImage(Font2Image[12], x, y);
    return 0;
  case '=':
    PutImage(Font2Image[13], x, y);
    return 0;
  case '>':
    PutImage(Font2Image[14], x, y);
    return 0;
  case '?':
    PutImage(Font2Image[15], x, y);
    return 0;
  case '@':
    PutImage(Font3Image[0], x, y);
    return 0;
  case 'A':
    PutImage(Font3Image[1], x, y);
    return 0;
  case 'B':
    PutImage(Font3Image[2], x, y);
    return 0;
  case 'C':
    PutImage(Font3Image[3], x, y);
    return 0;
  case 'D':
    PutImage(Font3Image[4], x, y);
    return 0;
  case 'E':
    PutImage(Font3Image[5], x, y);
    return 0;
  case 'F':
    PutImage(Font3Image[6], x, y);
    return 0;
  case 'G':
    PutImage(Font3Image[7], x, y);
    return 0;
  case 'H':
    PutImage(Font3Image[8], x, y);
    return 0;
  case 'I':
    PutImage(Font3Image[9], x, y);
    return 0;
  case 'J':
    PutImage(Font3Image[10], x, y);
    return 0;
  case 'K':
    PutImage(Font3Image[11], x, y);
    return 0;
  case 'L':
    PutImage(Font3Image[12], x, y);
    return 0;
  case 'M':
    PutImage(Font3Image[13], x, y);
    return 0;
  case 'N':
    PutImage(Font3Image[14], x, y);
    return 0;
  case 'O':
    PutImage(Font3Image[15], x, y);
    return 0;
  case 'P':
    PutImage(Font4Image[0], x, y);
    return 0;
  case 'Q':
    PutImage(Font4Image[1], x, y);
    return 0;
  case 'R':
    PutImage(Font4Image[2], x, y);
    return 0;
  case 'S':
    PutImage(Font4Image[3], x, y);
    return 0;
  case 'T':
    PutImage(Font4Image[4], x, y);
    return 0;
  case 'U':
    PutImage(Font4Image[5], x, y);
    return 0;
  case 'V':
    PutImage(Font4Image[6], x, y);
    return 0;
  case 'W':
    PutImage(Font4Image[7], x, y);
    return 0;
  case 'X':
    PutImage(Font4Image[8], x, y);
    return 0;
  case 'Y':
    PutImage(Font4Image[9], x, y);
    return 0;
  case 'Z':
    PutImage(Font4Image[10], x, y);
    return 0;
  case '[':
    PutImage(Font4Image[11], x, y);
    return 0;
  case '\\':
    PutImage(Font4Image[12], x, y);
    return 0;
  case ']':
    PutImage(Font4Image[13], x, y);
    return 0;
  case '^':
    PutImage(Font4Image[14], x, y);
    return 0;
  case '_':
    PutImage(Font4Image[15], x, y);
    return 0;
  case '`':
    PutImage(Font5Image[0], x, y);
    return 0;
  case 'a':
    PutImage(Font5Image[1], x, y);
    return 0;
  case 'b':
    PutImage(Font5Image[2], x, y);
    return 0;
  case 'c':
    PutImage(Font5Image[3], x, y);
    return 0;
  case 'd':
    PutImage(Font5Image[4], x, y);
    return 0;
  case 'e':
    PutImage(Font5Image[5], x, y);
    return 0;
  case 'f':
    PutImage(Font5Image[6], x, y);
    return 0;
  case 'g':
    PutImage(Font5Image[7], x, y);
    return 0;
  case 'h':
    PutImage(Font5Image[8], x, y);
    return 0;
  case 'i':
    PutImage(Font5Image[9], x, y);
    return 0;
  case 'j':
    PutImage(Font5Image[10], x, y);
    return 0;
  case 'k':
    PutImage(Font5Image[11], x, y);
    return 0;
  case 'l':
    PutImage(Font5Image[12], x, y);
    return 0;
  case 'm':
    PutImage(Font5Image[13], x, y);
    return 0;
  case 'n':
    PutImage(Font5Image[14], x, y);
    return 0;
  case 'o':
    PutImage(Font5Image[15], x, y);
    return 0;
  case 'p':
    PutImage(Font6Image[0], x, y);
    return 0;
  case 'q':
    PutImage(Font6Image[1], x, y);
    return 0;
  case 'r':
    PutImage(Font6Image[2], x, y);
    return 0;
  case 's':
    PutImage(Font6Image[3], x, y);
    return 0;
  case 't':
    PutImage(Font6Image[4], x, y);
    return 0;
  case 'u':
    PutImage(Font6Image[5], x, y);
    return 0;
  case 'v':
    PutImage(Font6Image[6], x, y);
    return 0;
  case 'w':
    PutImage(Font6Image[7], x, y);
    return 0;
  case 'x':
    PutImage(Font6Image[8], x, y);
    return 0;
  case 'y':
    PutImage(Font6Image[9], x, y);
    return 0;
  case 'z':
    PutImage(Font6Image[10], x, y);
    return 0;
  case '{':
    PutImage(Font6Image[11], x, y);
    return 0;
  case '|':
    PutImage(Font6Image[12], x, y);
    return 0;
  case '}':
    PutImage(Font6Image[13], x, y);
    return 0;
  case '~':
    PutImage(Font6Image[14], x, y);
    return 0;
  default:
    fprintf(stderr, "draw_char: unknown char found (\\x%x), ignoring", c);
    return -1;
  }
  
  /* should not reach here */
  return -2;
}

#ifdef HAVE_LIBSDL
/* return 0 on success, 1 on error */
int sdl_draw_rect(int x_src, int y_src, int w, int h)
{
  int x;
  int y;
  int status = 0;
  
  /* sanity check */
  if (w <= 0)
    return 0;
  if (h <= 0)
    return 0;
  
  if (SDL_LockSurface(dpy) != 0)
  {
    fprintf(stderr, "DrawRec: cannot lock dpy (%s)\n" ,SDL_GetError());
    return 1;
  }

  for (x = x_src; x <= x_src + w; x++)
  {
    if (sdl_draw_pixel(x, y_src) != 0)
      status = 1;
    if (sdl_draw_pixel(x, y_src + h) != 0)
      status = 1;
  }
  for (y = y_src; y <= y_src + h; y++)
  {
    if (sdl_draw_pixel(x_src, y) != 0)
      status = 1;
    if (sdl_draw_pixel(x_src + w, y) != 0)
      status = 1;
  }
  SDL_UnlockSurface(dpy);
  return status;
}

/* you must lock dpy before calling this function and unlock dpy
 * after calling it
 * return 0 on success, 1 on error
 */
static int sdl_draw_pixel(int x, int y)
{
  int bpp = 0;
  Uint32 pixel;
  Uint8 *p = NULL;

  /* sanity check */
  if ((x < 0) || (x >= FieldW + 20))
    return 0;
  if ((y < 0) || (y >= FieldH + 20))
    return 0;
  /* ugly hack leave window edges untouched */
  if (manage != NULL)
  {
    if ((x < 10) || (x >= FieldW + 10))
      return 0;
    if ((y < 10) || (y >= FieldH + 10))
      return 0;
  }
  

  bpp = dpy->format->BytesPerPixel;
  pixel = SDL_MapRGB(dpy->format, 255, 255, 255);
  p = ((Uint8 *) dpy->pixels) + y * dpy->pitch + x * bpp;

  switch(bpp)
  {
  case 1:
    *p = pixel;
    break;
  case 2:
    *(Uint16 *)p = pixel;
    break;
  case 3:
    /* 2^8 = 256, 2^16 = 65536 */
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
    {
      p[0] = (pixel / 65536) & 0xff;
      p[1] = (pixel / 256) & 0xff;
      p[2] = pixel & 0xff;
    }
    else
    {
      p[0] = pixel & 0xff;
      p[1] = (pixel / 256) & 0xff;
      p[2] = (pixel / 65536) & 0xff;
    }
    break;
  case 4:
    *(Uint32 *)p = pixel;
    break;
  default:
    fprintf(stderr, "sdl_draw_pixel: unknown bpp (%d)\n", bpp);
    return 1;
  }
  return 0;
}
#endif /* HAVE_LIBSDL */
