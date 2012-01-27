/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef TASK_ID_H
#define TASK_ID_H

/**
 *  Currently an unsigned integer; there should be no way to reach 4 billion tasks
 *  and even if the game does, hopefully most of the first few billion tasks have completed
 *  and the overflow will still give us unique identifiers.
 */
typedef unsigned int TaskId;

#endif
