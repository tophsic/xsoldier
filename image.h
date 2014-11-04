/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: image.h,v 1.4 2002/05/05 11:46:06 oohara Exp $ */

#ifndef IMAGE_H_INCLUDE
#define IMAGE_H_INCLUDE

#ifdef HAVE_LIBSDL
#include <SDL.h>
typedef SDL_Surface Image;
#else /* not HAVE_LIBSDL */
#include <X11/Xlib.h>
typedef struct {
    Pixmap pixmap;
    Pixmap mask;
    GC     maskgc;
    int    width, height;
} Image;
#endif /* not HAVE_LIBSDL */


extern void ReadFileToImage(const char *filename, Image **img);
extern void PutImage(Image *img, int x, int y);
extern void SplitImage(Image *img, Image ***imgs, int nsplit);
extern void FreeImage(Image *img);
extern void FreeImages(Image **imgs, int nimg);
extern Image **ImageInit(const char *filename, int split);

#ifdef GZIP
 #define GZ ".gz"
#else
 #define GZ
#endif

#endif /* IMAGE_H_INCLUDE */
