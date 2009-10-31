#ifndef __LINKED_LIST_NODE_H_
#define __LINKED_LIST_NODE_H_

/**
 * A templated linked list structure
 *
 * @author Noam Chitayat
 */
template<class C> struct LinkedListNode
{
   /** The data held in this node */
   C data;

   /** The linked node */
   LinkedListNode* next;

   /** Constructor. */
   LinkedListNode(C data, LinkedListNode* next) : data(data), next(next)
   {
   }
};

#endif
