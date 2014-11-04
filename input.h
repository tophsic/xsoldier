/* xsoldier, a shoot 'em up game with "not shooting" bonus
 * Copyright (C) 1997 Yuusuke HASHIMOTO <s945750@educ.info.kanagawa-u.ac.jp>
 * Copyright (C) 2002 Oohara Yuuma  <oohara@libra.interq.or.jp>
 *
 * This is a copyleft program.  See the file LICENSE for details.
 */
/* $Id: input.h,v 1.5 2002/04/20 01:42:25 oohara Exp $ */

#ifndef _INPUT_H_
#define _INPUT_H_
extern int input_init(void);
extern int event_handle(void);
extern int event_handle_opening(void);
extern int event_handle_ending(void);
#endif /* _INPUT_H_ */
