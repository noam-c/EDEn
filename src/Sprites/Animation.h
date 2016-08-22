/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>

#include "FrameSequence.h"

/**
 * An Animation iterates through different frame indices based on the time that
 * has passed between game loop frames. It iterates through a circularly linked
 * list loaded by the Spritesheet, and updates its position in the list based
 * on game loop steps.
 */
class Animation
{
   static const int MILLISECONDS_PER_FRAME = 100;

   /** The name of this animation. */
   const std::string& m_name;

   /** The sequence of frames in this animation. */
   const FrameSequence& m_frameSequence;

   /** The current frame that we are at in the list. */
   FrameSequence::const_iterator m_curr;

   /** The time left until the Animation needs to move to the next frame. */
   long m_timeToNextAnimation = MILLISECONDS_PER_FRAME;

public:
   /**
    * Constructor.
    * Creates an animation with a given linked list of frame indices.
    *
    * @param name The uniquely identifying name of the animation.
    * @param frameSequence The sequence of frames in a given animation.
    */
   Animation(const std::string& name, const FrameSequence& frameSequence);

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
   int getIndex() const;

   /**
    * @return The name of the animation.
    */
   const std::string& getName() const;
};

#endif
