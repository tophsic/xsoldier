/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: image.c,v 1.16 2002/05/06 04:13:30 oohara Exp $ */

/* DEBUG is defined in config.h */
#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_LIBSDL
#include <SDL.h>
#include <SDL_image.h>
#else /* not HAVE_LIBSDL */
#include <X11/Xlib.h>
#include <X11/xpm.h>
#endif /* not HAVE_LIBSDL */

#include "image.h"
#include "xsoldier.h"
#include "extern.h"

#ifndef HAVE_LIBSDL
static const char *XpmStatusToString(int status);

static const char *XpmStatusToString(int status)
{
    switch (status)
    {
      case XpmColorError:  return "XpmColorError";
      case XpmSuccess:     return "XpmSuccess";
      case XpmOpenFailed:  return "XpmOpenFailed";
      case XpmFileInvalid: return "XpmFileInvalid";
      case XpmNoMemory:    return "XpmNoMemory";
      case XpmColorFailed: return "XpmColorFailed";
    }
    return "Unknwon status";
}
#endif /* not HAVE_LIBSDL */

void ReadFileToImage(const char *filename, Image **img)
{
#ifdef HAVE_LIBSDL
  SDL_Surface *temp;
  temp = IMG_Load(filename);
  if (temp == NULL)
  {
    fprintf(stderr, "ReadFileToImage: IMG_Load(%s) failed (%s)\n",
            filename, SDL_GetError());
    exit(1);
  }

  *img = temp;
  
#else /* not HAVE_LIBSDL */
    XpmAttributes att;
    const char *FuncName = "ReadFileToImage";
    int status;
    /*
    *img = (Image *)malloc(sizeof(Image));
    */
    att.valuemask = XpmColormap;
    att.x_hotspot = 0U;
    att.y_hotspot = 0U;
    att.depth     = 8U;
    att.colormap  = cmap;

    /* I don't know why arg 3 of XpmReadFileToPixmap is not const */
    status = XpmReadFileToPixmap(dpy, WorkPixmap, (char *) filename, &((*img)->pixmap), &((*img)->mask), &att);
    if (status != XpmSuccess)
    {
	fprintf(stderr, "%s: [file error] can not read %s (%s)\n",
		FuncName, filename, XpmStatusToString(status));
	fflush(stderr);
	exit(1); 
    }
#ifdef DEBUG
    /* FIXME */
    if ((*img)->mask == None)
	fprintf(stderr, "%s: [format error] %s don't use None color\n", FuncName, filename);
#endif
    (*img)->width  = att.width;
    (*img)->height = att.height;

    if (((*img)->mask) != None)
	(*img)->maskgc = XCreateGC(dpy,(*img)->mask,0,0);
    else
	fprintf(stderr, "%s: [pixmap error] clip_mask is None!\n", FuncName);

    XpmFreeAttributes(&att);

#endif /* not HAVE_LIBSDL */
    return;
}

void SplitImage(Image *img, Image ***imgs, int nsplit)
{
  int width = 0;
  int height = 0;
  int i;
#ifdef DEBUG
    const char *FuncName = "SplitImage";
#endif

#ifdef HAVE_LIBSDL
  SDL_Rect temp;
  SDL_Rect temp_dest;
#endif /* HAVE_LIBSDL */

#ifdef HAVE_LIBSDL
  width = img->w;
  height = img->h / nsplit;

#ifdef DEBUG
  if ((img->h) % nsplit != 0)
  {
    fprintf(stderr, "%s: [warning] img->height (%d)/nsplit (%d) isn't just!\n", FuncName, img->h, nsplit);
    fflush(stderr);
  }
#endif /* DEBUG */

  (*imgs) = (Image **)malloc(sizeof(Image*)*nsplit);
  if (imgs == NULL)
  {
    perror("SplitImage: malloc failed");
    exit(1);
  }

  for (i=0; i<nsplit; i++)
  {
    (*imgs)[i] = SDL_CreateRGBSurface(0, width, height,
                                      dpy->format->BitsPerPixel,
                                      dpy->format->Rmask,
                                      dpy->format->Gmask,
                                      dpy->format->Bmask,
                                      dpy->format->Amask);
    if ((*imgs)[i] == NULL)
    {
      fprintf(stderr, "SplitImage: SDL_CreateRGBSurface failed (%s)\n",
              SDL_GetError());
      exit(1);
    }
    if (dpy->format->BitsPerPixel == 8)
      SDL_SetPalette((*imgs)[i], SDL_LOGPAL|SDL_PHYSPAL,
                     dpy->format->palette->colors,
                     0, dpy->format->palette->ncolors);
    
  }
  
  /* clear the window with white for debug display */
  /*
  SDL_FillRect(dpy, NULL, SDL_MapRGB(dpy->format, 255, 255, 255));
  */

  for (i=0; i<nsplit; i++)
  {
    int x, y;

    x = 0;
    y = height * i;

    temp.x = x;
    temp.y = y;
    temp.w = width;
    temp.h = height;
    temp_dest.x = 0;
    temp_dest.y = 0;
    temp_dest.w = width;
    temp_dest.h = height;
    /*
    SDL_UnlockSurface(img);
    SDL_UnlockSurface((*imgs)[i]);
    */
    if (SDL_BlitSurface(img, &temp, (*imgs)[i], &temp_dest) != 0)
    {
      fprintf(stderr, "SplitImage: SDL_BlitSurface failed (%s)\n",
              SDL_GetError());
      exit(1);
    }
    
    if (SDL_SetColorKey((*imgs)[i], SDL_SRCCOLORKEY,
                        SDL_MapRGB(dpy->format, 0, 0, 0)) != 0)
    {
      fprintf(stderr, "SplitImage: SDL_SetColorKey failed (%s)\n",
              SDL_GetError());
      exit(1);
    }
    
    /* debug display */
    /*
    temp_dest.x = 50 + width * i;
    temp_dest.y = 100;
    SDL_BlitSurface((*imgs)[i], NULL, dpy, &temp_dest);
    SDL_Flip(dpy);
    */
  }
  
#else /* not HAVE_LIBSDL */

    GC  gc8, gc1;

    width  = img->width;
    height = (img->height) / nsplit;
#ifdef DEBUG
    if ((img->height) % nsplit != 0)
    {
	fprintf(stderr, "%s: [warning] img->height (%d)/nsplit (%d) isn't just!\n", FuncName, img->height, nsplit);
	fflush(stderr);
    }
#endif

    gc8 = XCreateGC(dpy,img->pixmap, 0L,NULL);
    gc1 = XCreateGC(dpy,img->mask,   0L,NULL);

    XSetGraphicsExposures(dpy, gc8, False);
    XSetGraphicsExposures(dpy, gc1, False);

    (*imgs) = (Image **)malloc(sizeof(Image*)*nsplit);

    for (i=0; i<nsplit; i++)
	(*imgs)[i] = (Image *)malloc(sizeof(Image));

    for (i=0; i<nsplit; i++)
    {
	int x, y;

	x = 0;
	y = height * i;

	(*imgs)[i]->pixmap = XCreatePixmap(dpy,RootWindow(dpy,0),width,height,DefaultDepth(dpy,0));
	XCopyArea(dpy,img->pixmap,(*imgs)[i]->pixmap,gc8,x,y,width,height,0,0);

	(*imgs)[i]->mask = XCreatePixmap(dpy,RootWindow(dpy,0),width,height,1);
	XCopyArea(dpy,img->mask,  (*imgs)[i]->mask,gc1,x,y,width,height,0,0);

	(*imgs)[i]->maskgc = XCreateGC(dpy,WorkPixmap,0,0);
	XSetClipMask(dpy,(*imgs)[i]->maskgc,(*imgs)[i]->mask);

	(*imgs)[i]->width  = width;
	(*imgs)[i]->height = height;
    }

    XFreeGC(dpy,gc8);
    XFreeGC(dpy,gc1);
    XFlush(dpy);
#endif /* not HAVE_LIBSDL */
    return;
}

void PutImage(Image *img, int x, int y)
{
#ifdef HAVE_LIBSDL
  SDL_Rect temp;

  /* use double-standard here to keep the 10 pixel boundary */
  temp.x = x + 10;
  temp.y = y + 10;
  temp.w = img->w;
  temp.h = img->h;

  if (SDL_BlitSurface(img, NULL, dpy, &temp) != 0)
  {
    fprintf(stderr, "SplitImage: SDL_BlitSurface failed (%s)\n",
            SDL_GetError());
  }

#else  /* not HAVE_LIBSDL */
    XSetClipOrigin(dpy,img->maskgc,x,y);
    XCopyArea(dpy,img->pixmap,WorkPixmap,img->maskgc,0,0,img->width,img->height,x,y);
#endif /* not HAVE_LIBSDL */
}

void FreeImage(Image *img)
{
#ifdef HAVE_LIBSDL
  SDL_FreeSurface(img);
#else  /* not HAVE_LIBSDL */
    XFreePixmap(dpy, img->pixmap);
    XFreePixmap(dpy, img->mask);
    XFreeGC(dpy, img->maskgc);
    free(img);
#endif /* not HAVE_LIBSDL */
    return;
}

void FreeImages(Image **imgs, int nimg)
{
  while (nimg)
    FreeImage(imgs[--nimg]);
  free(imgs);
  return;
}

Image **ImageInit(const char *filename, int split)
{
    Image *Digit;
    Image **Digits;

    /* if we are using SDL, we don't need to malloc here because IMG_Load
     * do it for us
     */
#ifndef HAVE_LIBSDL
    Digit = (Image *)malloc(sizeof(Image));
#endif /* not HAVE_LIBSDL */
    ReadFileToImage(filename,&Digit);
    SplitImage(Digit,&Digits,split);

    FreeImage(Digit);
    return Digits;
}
