/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: score.c,v 1.11 2011/08/12 14:29:02 oohara Exp $ */

/* DEBUG and DUPSCORE are defined in config.h */
#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
*/

/* strcmp */
#include <string.h>
/* isprint */
#include <ctype.h>

#include "image.h"
#include "xsoldier.h"

#include "extern.h"

#include "score.h"

static void fake_score_entry(Record *rec, int n, int score);
static void copy_Record_to_PlayerData(Record *rec, PlayerData *p);
static int compare_entry(const void *a, const void *b);


/* read entries from high score file and save them in p
 * returns 0 on success, -1 on error
 */
int ReadHiscore(PlayerData *p)
{
  FILE *fp;
  char buff[BUFSIZ];
  Record rec[10];
  int i;
  int j;
  int status = 0;

  char name[32];
  int score;
  int loop;
  int stage;

  /* initialize the entries with random records */
  for (i = 0; i <= 9; i++)
    fake_score_entry(rec, i, (11 - i) * 10000);

  /* open the score file */
  if ((fp = fopen(SCORE "/" SCOREFILE, "r")) == NULL)
  {
    fprintf(stderr,"ReadHiscore: can not open score file\n");
    copy_Record_to_PlayerData(rec, p);
    return -1;
  }

  /* read each record --- one line for one record */
  for (i = 0; (i <= 9) && (fgets(buff, BUFSIZ, fp) != NULL); i++)
  {
    if (sscanf(buff, "%d %d %d %31s", &score, &loop, &stage, name) == 4)
    {
      rec[i].score = score;
      rec[i].loop  = loop;
      rec[i].stage = stage;
      /* check if the player name consists of only printable chars */
      for (j = 0; (j <= sizeof(name) - 1) && (name[j] != '\0'); j++)
        if (!isprint(name[j]))
        {
          fprintf(stderr, "warning: non-printable char found in the name of "
                  "entry %d, char %d (\\x%x), replacing it with ?\n",
                  i + 1, j, name[j]);
          name[j] = '?';
        }

      strncpy(rec[i].name, name, sizeof(rec[i].name));
#ifdef DEBUG
      fprintf(stderr, "entry %d read: %d %d %d %s\n", i + 1, rec[i].score,
              rec[i].loop, rec[i].stage, rec[i].name);
#endif /* DEBUG */
    }
    else
    {
      fprintf(stderr, "warning: entry %d of the high score file is "
              "broken so ignored\n", i + 1);
      status = -1;
    }
  }

  /* close the score file */
  if (fclose(fp) == EOF)
  {
    fprintf(stderr, "ReadHiscore: can't close the score file\n");
    status = -1;
  }

  /* warn if there are only 9 or less entries */
  if (i != 10)
  {
    fprintf(stderr, "warning: the score file has only %d ", i);
    /* plural hack */
    if (i >= 2)
      fprintf(stderr, " entries\n");
    else
      fprintf(stderr, " entry\n");
    status = -1;
  }

  /* sort the entries */
  qsort(rec, 10, sizeof(Record), compare_entry);
  
  copy_Record_to_PlayerData(rec, p);
  return status;
}

/* checks if the score of the current player can be in the high score list
 * and merge if necessary
 * assumes p is already sorted
 * returns the rank of the current player is s/he gets in the list,
 * -1 if out of the list
*/

int MergeHiscore(PlayerData *p)
{
  int rank = -1;
  int del = 10;
  int i;

#ifndef DUPSCORE
  size_t temp;
#endif /* not DUPSCORE */

  /* ugly hack to cope with a braindamage
   * this is necessary so that we can check the score correctly
   * even if the player quit the game with q key
   */

  p->Rec[0].loop = manage->Loop;
  p->Rec[0].stage = manage->Stage;

#ifndef DUPSCORE
  /* check if a entry with the same name is already in the list
   * if there is, find the highest one
   */
  for (i = 1; i <= 10; i++)
  {
    temp = sizeof(p->Rec[0].name);
    if (temp > sizeof(p->Rec[i].name))
      temp = sizeof(p->Rec[i].name);
    if (strncmp(p->Rec[0].name, p->Rec[i].name, temp) == 0)
    {
      del = i;
      break;
    }
  }
#endif /* not DUPSCORE */

  /* p is already sorted */
  for (i = 1; i<=10; i++)
  {
    if (p->Rec[0].score > p->Rec[i].score)
    {
      rank = i;
      break;
    }
  }

  /* if the current player is out of the top 10 */
  if (rank == -1)
    return -1;

#ifndef DUPSCORE
  /* if there is already an entry with the same name and a better score */
  if (rank > del)
    return -1;
#endif /* not DUPSCORE */

  /* merge the score of the current player */
  for (i = del; i > rank; i--)
  {
#ifdef DEBUG
    printf("rank[%d] <- rank[%d]\n", i, i - 1);
#endif
    p->Rec[i].score = p->Rec[i - 1].score;
    p->Rec[i].loop  = p->Rec[i - 1].loop;
    p->Rec[i].stage = p->Rec[i - 1].stage;
    strncpy(p->Rec[i].name, p->Rec[i - 1].name, sizeof(p->Rec[i].name));
  }
#ifdef DEBUG	
  printf("rank[%d] <- rank[0]\n", rank);
  fprintf(stderr, "%d %d %d %s\n", p->Rec[0].score, p->Rec[0].loop,
          p->Rec[0].stage, p->Rec[0].name);
#endif
  p->Rec[rank].score = p->Rec[0].score;
  p->Rec[rank].loop  = p->Rec[0].loop;
  p->Rec[rank].stage = p->Rec[0].stage;
  strncpy(p->Rec[rank].name, p->Rec[0].name, sizeof(p->Rec[rank].name));

  return rank;
}

/* write the high score list to the disk
 * return 0 on success, -1 on failure
 */
int WriteHiscore(PlayerData *p)
{
  FILE *fp;
  int i;
  int status = 0;

  /* open the score file */
  if ((fp = fopen(SCORE "/" SCOREFILE,"w")) == NULL)
  {
    fprintf(stderr,"WriteHiscore: can not open score file\n");
    return -1;
  }

  /* write to the disk */
  for (i = 1; i <= 10; i++)
  {
    if (fprintf(fp, "%d %d %d %s\n", p->Rec[i].score, p->Rec[i].loop,
                p->Rec[i].stage, p->Rec[i].name) < 0)
    {
      fprintf(stderr, "WriteHiscore: can not write the score file\n");
      fprintf(stderr, "failed entry:\n");
      fprintf(stderr, "%d %d %d %s\n", p->Rec[i].score, p->Rec[i].loop,
              p->Rec[i].stage, p->Rec[i].name);
      status = -1;
   }
  }

  /* close the score file */
  if (fclose(fp) == EOF)
  {
    fprintf(stderr, "WriteHiscore: can't close the score file\n");
    status = -1;
  }

  return status;
}

/* generate a fake record for the high score list
 * note that rec begins with 0
 */
static void fake_score_entry(Record *rec, int n, int score)
{
  rec[n].score = score;
  rec[n].loop  = (10 - n) / 3;
  if (rec[n].loop < 1)
    rec[n].loop = 1;
  rec[n].stage = rand() % 8 + 1;
  snprintf(rec[n].name, sizeof(rec[n].name) - 1, "(unknown)");
}

static void copy_Record_to_PlayerData(Record *rec, PlayerData *p)
{
  int i;
  for (i = 1; i <= 10; i++)
    p->Rec[i] = rec[i - 1];
}

static int compare_entry(const void *a, const void *b)
{
  const Record *rec_a = (const Record *) a;
  const Record *rec_b = (const Record *) b;
  if (rec_a->score > rec_b->score)
    return -1;
  if (rec_a->score < rec_b->score)
    return 1;

  if (rec_a->loop > rec_b->loop)
    return -1;
  if (rec_a->loop < rec_b->loop)
    return 1;

  if (rec_a->stage > rec_b->stage)
    return -1;
  if (rec_a->stage < rec_b->stage)
    return 1;

  return strcmp (rec_a->name, rec_b->name);
}
