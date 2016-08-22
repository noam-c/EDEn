/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef TRANSITION_H
#define TRANSITION_H

#include <memory>

#include "GameState.h"
#include "Texture.h"

class Transition
{
   protected:
      /** The total time elapsed since the transition began. */
      long m_totalTime = 0;

      /** The length of time that the transition will take to complete. */
      const long m_transitionLength;

      /** The progress (value between 0 and 1) through the transition. */
      double m_progress = 1.0f;

      /** A texture for the old state. */
      Texture m_oldStateTexture;

      /** A texture for the new state. */
      Texture m_newStateTexture;

   public:
      /**
       * Constructor.
       *
       * @param oldStateTexture The old texture that the transition will be moving from.
       * @param transitionLength An optional length (in milliseconds) for the transition.
       */
      Transition(Texture&& oldStateTexture, long transitionLength = 1000);
   
      /**
       * Constructor.
       *
       * @param oldStateTexture The old texture that the transition will be moving from.
       * @param newStateTexture The new texture that the transition will be moving to.
       * @param transitionLength An optional length (in milliseconds) for the transition.
       */
      Transition(Texture&& oldStateTexture, Texture&& newStateTexture, long transitionLength = 1000);

      /**
       * Destructor.
       */
      virtual ~Transition();

      /**
       * Update the transition to the right timeframe.
       */
      virtual bool step(long timePassed);

      /**
       * Draw the transition.
       */
      virtual void draw() = 0;
};

#endif
