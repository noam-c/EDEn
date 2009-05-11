#ifndef __TICKET_ID_H_
#define __TICKET_ID_H_

/** A TicketId is a unique identifier for engine instructions that occur across
 *  many frames of logic. These tickets are handed to the scheduler, which blocks
 *  Threads waiting on the instruction and resumes them when the instruction is finished.
 *
 *  Currently an unsigned integer; there should be no way to reach 4 billion instructions
 *  and even if the game does, hopefully most of the first billion instructions have completed
 *  and the overflow will still give us unique identifiers.
 */
typedef unsigned int TicketId;

#endif
