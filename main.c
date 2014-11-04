/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: main.c,v 1.31 2006/09/16 09:20:54 oohara Exp $ */

/* DEBUG and JSTK are defined in config.h */
#include <config.h>
/* time */
#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#include "xsoldier.h"
#include "manage.h"
#include "opening.h"
#include "game.h"
#include "ending.h"
#include "player.h"
/* DeleteAllStar */
#include "star.h"
#include "score.h"
#include "wait.h"
#include "graphic.h"
#include "input.h"

/* define all "extern" here */
#define EXTERN_DEF
#include "extern.h"

/* local functions */
static void arginit(int argc, char *argv[]);
static void usage(void);

static void init(void);
static void clean(void);
static void sig_handle(int arg);

/* file-global variables */
static char command[32];
static int scoreOK = True;
static int putscore = False;
static int w_time = -1;

static int maxlevel_temp = False;
static int start_stage = 1;
static int start_loop = 1;
static int start_ship = 5;
static int maxpower_temp = False;
static int nopausemessage_temp = False;

int main(int argc, char *argv[])
{
  display[0] = '\0';
  manage = NULL;

    arginit(argc,argv);

    player = NewPlayerData();
    if (putscore == True)
    {
	int i;
	printf("      --- top 10 soldiers ---\n");
	printf("Name                 Stage Score\n");
	for (i=1; i<=10; i++)
	    printf("%-20.20s %2d-%2d %8d\n",
		   player->Rec[i].name,
		   player->Rec[i].loop,player->Rec[i].stage,
		   player->Rec[i].score);

	exit(0);
    }
    if (scoreOK == False)
	fprintf(stderr,"caution: no ranking mode!\n");

    init();
    manage = NewManage(256,256);
    if (maxlevel_temp == True)
      manage->flag_maxlevel = True;
    if (maxpower_temp == True)
      manage->start_power = 30;
    else
      manage->start_power = 0;

    if (nopausemessage_temp == True)
      manage->flag_nopausemessage = True;
    
#ifndef HAVE_LIBSDL
    XMapWindow(dpy,win);
    XMapWindow(dpy,root);

    XFlush(dpy);
#endif /* not HAVE_LIBSDL */

    while (!(manage->program_should_quit))
    {
	keymask = 0;

	player->Rec[0].score = 0;
	player->Ships = start_ship;
	player->Next = FIRST1UP;
	manage->Loop = start_loop;
        manage->Stage = start_stage;
        
	if (Opening() == -1)
	    break;

	while (1)
	{
	    keymask = 0;
	    ResetManage(manage);

	    if (mainLoop() == 0)
              /* game over */
		break;
	    Ending();
	    manage->Loop++;
            manage->Stage = 1;
            if (manage->Loop >3)
              break;
	}

	if (scoreOK == True)
	{
	    if (MergeHiscore(player) > 0)
		WriteHiscore(player);
	}
    }

    DeleteManage(manage);
    free(player);
    clean();
    return 0;
}

static void arginit(int argc, char *argv[])
{
    char *argv0;
    int i;
    int a;
#ifdef DEBUG
    int b;
#endif /* DEBUG */

    if ((argv0=strrchr(argv[0],'/')) != NULL)
        argv0++;
    else
        argv0 = argv[0];
    /* copy command name */
    strncpy(command,argv0, sizeof(command)-1);

    for (i=1; i<argc; i++)
    {
	if ((strcmp(argv[i],"-display")==0) || (strcmp(argv[i],"-d")==0))
	{
	    if (i < argc-1)
            {
              strncpy(display,argv[i + 1],sizeof(display));
              if (display[sizeof(display) - 1] != '\0')
              {
                fprintf(stderr, "warning: display name (arg %d) is too long, ",
                        i + 1);
                display[sizeof(display) - 1] = '\0';
                fprintf(stderr, "truncated to %d chars\n",
                        sizeof(display) - 1);
              }
              i++;
            }
            else
            {
              fprintf(stderr, "no display specified for -display (arg %d)\n",
                      i);
              exit(1);
            }
	}
        else if ((strcmp(argv[i],"-wait")==0) || (strcmp(argv[i],"-w")==0))
	{
	    if (i < argc-1)
	    {
              if ((sscanf(argv[i+1], "%d", &a) == 1) && (a >= 1))
              {
                w_time = a;
                if (w_time > WAIT)
                  scoreOK = False;
                i++;
              }
              else
              {
                fprintf(stderr, "wait (arg %d) must be a positive integer.\n",
                        i + 1);
                exit(1);
              }
            }
            else
            {
              fprintf(stderr, "no wait specified for -wait (arg %d)\n", i);
              exit(1);
            }
	}
        else if ((strcmp(argv[i],"-cmap")==0) || (strcmp(argv[i],"-c")==0))
        {
          /* ignore */
          ; 
	}
        else if ((strcmp(argv[i],"-score")==0) || (strcmp(argv[i],"-s")==0))
	    putscore = True;
        else if ((strcmp(argv[i],"-help")==0) || (strcmp(argv[i],"-h")==0))
            usage();
        else if (strcmp(argv[i],"-maxlevel")==0)
	    maxlevel_temp = True;
        else if (strcmp(argv[i],"-nopausemessage")==0)
	    nopausemessage_temp = True;
#ifdef DEBUG
        else if (strcmp(argv[i],"-stage") == 0)
        {
          if (i + 1 < argc)
          {
            if (sscanf(argv[i+1], "%d-%d", &a, &b) == 2)
            {
              if ((a < 1) || (a > 3))
              {
                fprintf(stderr, "the loop number (arg %d) must be between "
                        "1 and 3\n", i + 1);
                exit(1);
              }
              else if ((b < 1) || (b > 8))
              {
                fprintf(stderr, "the stage number (arg %d) must be between "
                        "1 and 8\n", i + 1);
                exit(1);
              }
              else
              {
                scoreOK = False;
                start_loop = a;
                start_stage = b;
                i++;
              }
            }
            else if (sscanf(argv[i+1], "%d", &b) == 1)
            {
              if ((b >= 1) && (b <= 8))
              {
                scoreOK = False;
                start_stage = b;
                i++;
              }
              else
              {
                fprintf(stderr, "the stage number (arg %d) must be between "
                        "1 and 8\n", i + 1);
                exit(1);
              }
            }
            else
            {
              fprintf(stderr, "strange stage number (arg %d) for -stage\n",
                      i + 1);
              exit(1);
            }
          }
          else
          {
            fprintf(stderr, "missing stage number for -stage (arg %d)\n", i);
            exit(1);
          }
        }
        else if (strcmp(argv[i],"-loop") == 0)
        {
          if (i + 1 < argc)
          {
            if (sscanf(argv[i+1], "%d", &a) == 1)
            {
              if ((a >= 1) && (a <= 3))
              {
                scoreOK = False;
                start_loop = a;
                i++;
              }
              else
              {
                fprintf(stderr, "loop number (arg %d) must be between "
                        "1 and 3\n", i + 1);
                exit(1);
              }
            }
            else
            {
              fprintf(stderr, "strange loop number (arg %d) for -loop\n",
                      i + 1);
              exit(1);
            }
          }
          else
          {
            fprintf(stderr, "missing loop number for -loop (arg %d)\n", i);
            exit(1);
          }
        }
        else if (strcmp(argv[i],"-ship") == 0)
        {
          if (i + 1 < argc)
          {
            if (sscanf(argv[i+1], "%d", &a) == 1)
            {
              if ((a >= 0) && (a <= 99))
              {
                scoreOK = False;
                start_ship = a;
                i++;
              }
              else
              {
                fprintf(stderr, "ship number (arg %d) must be between "
                        "0 and 99\n", i + 1);
                exit(1);
              }
            } 
            else
            {
              fprintf(stderr, "strange ship number (arg %d) for -ship\n",
                      i + 1);
              exit(1);
            }
          }
          else
          {
            fprintf(stderr, "missing ship number for -ship (arg %d)\n", i);
            exit(1);
          }
        }
        else if (strcmp(argv[i],"-maxpower")==0)
        {
          scoreOK = False;
          maxpower_temp = True;
        }
#endif /* DEBUG */
        else
        {
          fprintf(stderr, "strange option (arg %d)\n", i);
          exit(1);
        }
    }
}

static void usage(void)
{
    fprintf(stderr,"Usage: %s [-display <name>] [-wait <n>] [-score] [-help] [-maxlevel]",command);
#ifdef DEBUG
    fprintf(stderr, "[-stage <L-S>] [-stage <S>] [-loop <L>] [-ship <N>] [maxpower]");
#endif /* DEBUG*/
    fprintf(stderr, "\n");

    fprintf(stderr,"\t display <name>   ... name of the X server to play game.\n");
    fprintf(stderr,"\t wait <n>         ... set wait time. (no ranking) <default is %d>\n",WAIT);
    fprintf(stderr,"\t score            ... print top 10 soldiers.\n");
    fprintf(stderr,"\t help             ... this message.\n");
    fprintf(stderr,"\t maxlevel         ... force max level enemy attack.\n");
#ifdef DEBUG
    fprintf(stderr,"debug-only options\n");
    fprintf(stderr,"\t stage <L-S>      ... start stage S [1-8] of loop L [1-3]. (no ranking)\n");
    fprintf(stderr,"\t stage <S>        ... start stage S [1-8]. (no ranking)\n");
    fprintf(stderr,"\t loop <L>         ... start loop L [1-3]. (no ranking)\n");
    fprintf(stderr,"\t ship <n>         ... start with n [0-99] ships. (no ranking)\n");
    fprintf(stderr,"\t maxpower         ... start with max power. (no ranking)\n");
#endif /* DEBUG */
    exit(0);
}

static void init(void)
{
    struct itimerval value, ovalue;
    struct sigaction sig_act;

    /* set wait */
    signal_delivered = 1;
    if (w_time < 0)
	waittime = WAIT;
    else
	waittime = w_time;

    memset(&sig_act, 0, sizeof(sig_act));
    sig_act.sa_handler = sig_handle;
    sigaction(SIGALRM, &sig_act, 0);
    value.it_interval.tv_sec = 0;
    value.it_interval.tv_usec = waittime;
    value.it_value.tv_sec = 0;
    value.it_value.tv_usec = waittime;
    setitimer(ITIMER_REAL, &value, &ovalue);

    FieldW  = 500;
    FieldH  = 650;

    /* you must call graphic_init() first because it calls SDL_Init */
    graphic_init();
    input_init();

    srand((unsigned)time(NULL));
}

static void clean(void)
{
    graphic_finish();
}

static void sig_handle(int arg)
{
    signal_delivered = 1;
}
