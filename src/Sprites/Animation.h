#ifndef __ANIMATION_H_
#define __ANIMATION_H_

template<class C> struct LinkedListNode;

/**
 * Holds integers in a linked list structure to signify frames of an animation
 *
 * @author Noam Chitayat
 */
typedef LinkedListNode<int> FrameNode;

/**
 * \todo Document.
 */
class Animation
{
   FrameNode* curr;
   long timeToNextAnimation;

   public:
      Animation(FrameNode* frameNode);
      void update(long timePassed);
      int getIndex();
};

#endif
