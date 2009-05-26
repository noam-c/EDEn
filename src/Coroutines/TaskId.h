#ifndef __TASK_ID_H_
#define __TASK_ID_H_

/**
 *  Currently an unsigned integer; there should be no way to reach 4 billion tasks
 *  and even if the game does, hopefully most of the first few billion tasks have completed
 *  and the overflow will still give us unique identifiers.
 */
typedef unsigned int TaskId;

#endif
