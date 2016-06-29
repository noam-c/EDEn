/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef RANDOM_TRANSITION_GENERATOR_H
#define RANDOM_TRANSITION_GENERATOR_H

#include <memory>

class GameContext;
class GameState;
class Texture;

/**
 * A factory that generates a random transition from the existing transition set.
 *
 * @author Bobby Richter
 */
class RandomTransitionGenerator
{
  public:
     /**
      * Creates a new GameState randomly chosen from a set of available transitions.
      *
      * @param gameContext The context containing the execution stack managing this transition.
      * @param oldState The old state that the transition will be moving from.
      * @param newState The new state that the transition will be moving to.
      * @param transitionLength An optional length (in milliseconds) for the transition.
      *
      * @return a newly generated instance of the randomly selected transition state.
      */
      static std::shared_ptr<GameState> create(GameContext& gameContext, Texture&& oldStateTexture, Texture&& newStateTexture, long transitionLength = 1000);
};

#endif
