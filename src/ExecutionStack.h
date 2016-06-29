/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef EXECUTION_STACK_H
#define EXECUTION_STACK_H

#include "Singleton.h"
#include <memory>
#include <stack>

class GameState;
class Scheduler;

/**
 * The heart of the game's execution.
 * Holds different states of the game (Title Screen, Tile Engine, etc.)
 * and allows for easy change of state.
 * Main functionality is calling advanceFrame and drawFrame, and destroying finished states in the execute() function.
 *
 * @author Noam Chitayat
 */
class ExecutionStack final
{
   /**
    * The stack that holds the various states that the player
    * has transitioned through.
    */
   std::stack<std::shared_ptr<GameState>> m_stateStack;

   /**
    * The next state to push after the current transition completes.
    */
   std::shared_ptr<GameState> m_nextState = nullptr;

   /**
    * Remove and delete the most recent state pushed on the stack.
    */
   void popState();

   public:
      /**
       * Pushes (and activates) a new game state.
       * After this method call, newState is responsible
       * for all drawing and logic steps until it is finished
       * or another state is pushed
       *
       * @param newState the new state of the game
       * @param transitionState an optional transition state used to introduce the new state visually
       */
      void pushState(std::shared_ptr<GameState> newState, std::shared_ptr<GameState> transitionState = nullptr);

      /**
       * @return The currently executing game state.
       */
      std::shared_ptr<GameState>& getCurrentState();

      /**
       * @return The currently executing game state.
       */
      const std::shared_ptr<GameState>& getCurrentState() const;

      /**
       * @return The currently executing game state's scheduler.
       */
      Scheduler* getCurrentScheduler() const;

      /**
       * Execute the game loop.
       * Step through the state logic. If the logic returns true
       * then the state is not ready to terminate, so run its draw step.
       * Otherwise, pop the stack and activate the next most recent state.
       * Keep going until there are no more states, and then quit.
       */
      void execute();
};

#endif
