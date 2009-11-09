#ifndef __LINKED_LIST_NODE_H_
#define __LINKED_LIST_NODE_H_

#ifndef NULL
   #define NULL 0
#endif

/**
 * A templated linked list structure.
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

   /**
    * Destructor.
    *
    * Recursively deletes linked nodes until an empty link is
    * encountered (base case).
    */
   ~LinkedListNode()
   {
      if(next != NULL)
      {
         delete next;
      }
   }
};

#endif
