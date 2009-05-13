#ifndef __TASK_ID_H_
#define __TASK_ID_H_

/** A TaskId is a unique identifier for engine instructions that occur across
 *  many frames of logic. These IDs are handed to the Scheduler, which blocks
 *  Threads waiting on the instruction and resumes them when the instruction is finished.
 *
 *  Currently an unsigned integer; there should be no way to reach 4 billion tasks
 *  and even if the game does, hopefully most of the first few billion tasks have completed
 *  and the overflow will still give us unique identifiers.
 */
typedef unsigned int TaskId;

#endif
