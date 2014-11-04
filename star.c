/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: star.c,v 1.8 2002/05/06 04:26:20 oohara Exp $ */

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
/*
#include <X11/Xlib.h>
*/

#include "image.h"
#include "xsoldier.h"
#include "extern.h"
#include "star.h"

#define MAXSTAR 4

typedef struct {
  /* coordinates of the star */
    int x, y;
  /* pattern number of the star */
    int cpat;
} Point;

typedef struct {
    Point *point;
    Image **image;
    unsigned int sW, sH;

    int pattern;
    int speed;
    int nstar;
} Star;

static Star     star[MAXSTAR];
static int      cust;
static unsigned int dW, dH;

void InitStarModule(unsigned int w, unsigned int h)
{
    dW = w; dH = h;
    cust = 0;
    return;
}

int CreateStar(const char *filename, int pattern, int speed, int nstar)
{
    Image *tmp;
    Star  *S = &(star[cust]);
    int i;

    if (cust >= MAXSTAR)
    {
	fprintf(stderr, "CreateStar: can't create star!\n");
	fflush(stderr);
	return -1;
    }

    /* if we are using SDL, we don't need to malloc here because IMG_Load
     * do it for us
     */
#ifndef HAVE_LIBSDL
    tmp = (Image *)malloc(sizeof(Image));
#endif /* not HAVE_LIBSDL */
    ReadFileToImage(filename,&tmp);
    SplitImage(tmp,&(S->image),pattern);
    FreeImage(tmp);

    S->pattern   = pattern;
    S->speed     = speed;
    S->nstar     = nstar;

    S->point = (Point *)malloc(sizeof(Point) * nstar);
#ifdef HAVE_LIBSDL
    S->sW    = dW+(S->image[0])->w;
    S->sH    = dH+(S->image[0])->h;
#else /* not HAVE_LIBSDL */
    S->sW    = dW+(S->image[0])->width;
    S->sH    = dH+(S->image[0])->height;
#endif /* not HAVE_LIBSDL */
    for (i=0; i<nstar; i++)
    {
	S->point[i].cpat = rand() % pattern;
	S->point[i].x    = rand() % S->sW;
	S->point[i].y    = rand() % S->sH;
    }
    
    return cust++;
}

void DrawStar(int id)
{
    static Image *I;
    static Star *S;
    static Point *p;
    int i;


    S = &(star[id]);
    for (i=0; i<S->nstar; i++)
    {
	p = &(S->point[i]);
	I = S->image[(p->cpat)++];
#ifdef HAVE_LIBSDL
	PutImage(I,p->x - I->w, p->y - I->h);
#else /* not HAVE_LIBSDL */
	PutImage(I,p->x - I->width, p->y - I->height);
#endif /* not HAVE_LIBSDL */
	if (p->cpat >= S->pattern) p->cpat = 0;
    }
    
    for (i=0; i<S->nstar; i++)
    {
	p = &(S->point[i]);
	p->y += S->speed;
	if (p->y < 0 || p->y > S->sH)
	{
	    p->x = rand() % S->sW;
	    p->y = (S->speed > 0)?0:S->sH;
	}
    }

    return;
}

void ChangeStarParameter(int id, int speed)
{
    static Star *S;

    S = &(star[id]);
    S->speed     = speed;
    return;
}

int DeleteAllStar(void)
{
  int i;
  for (i = 0; i < cust; i++)
  {
    FreeImages(star[i].image, star[i].pattern);
    free(star[i].point);
  }
  
  return 0;
}
