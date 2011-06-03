#ifndef ANIMATION_H
#define ANIMATION_H

template<class C> struct LinkedListNode;

/**
 * Holds integers in a linked list structure to signify frames of an animation
 *
 * @author Noam Chitayat
 */
typedef LinkedListNode<int> FrameNode;

/**
 * An Animation iterates through different frame indices based on the time that
 * has passed between game loop frames. It iterates through a circularly linked
 * list loaded by the Spritesheet, and updates its position in the list based
 * on game loop steps.
 */
class Animation
{
   /** The current frame that we are at in the list. */
   FrameNode* curr;

   /** The time left until the Animation needs to move to the next frame. */
   long timeToNextAnimation;

   public:
      /**
       * Constructor.
       * Creates an animation with a given linked list of frame indices.
       *
       * @param frameNode The list of frames in a given animation.
       */
      Animation(FrameNode* frameNode);

      /**
       * Updates the animation based on the time that has passed since the
       * last call.
       *
       * @param timePassed The amount of time that has passed.
       */
      void update(long timePassed);

      /**
       * @return The current node index (the index for the frame this animation
       *         is currently on).
       */
      int getIndex();

      /**
       * Destructor.
       */
      ~Animation();
};

#endif
