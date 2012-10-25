/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <string>

class ExecutionStack;

namespace Rocket
{
   namespace Core
   {
      class Context;
   }
};

union SDL_Event;

/**
 * This class abstractly represents a potential state for the game to enter
 * States can include "Title Screen", "Battle", "Field" and more.
 * Each GameState has a step function to perform state logic and a 
 * draw function to perform graphics work
 *
 * Game states are pushed onto the ExecutionStack, which executes the most
 * recent state of the game by causing the state to step forward in its logic, then draw to screen.
 *
 * The activate method is called by the Execution Stack whenever
 * the state reaches the top of the stack.
 *
 * @author Noam Chitayat
 */
class GameState
{
   protected:
      /** The execution stack that the state belongs to. */
      ExecutionStack& executionStack;
   
      /** The Rocket context for any GUI created by the state. */
      Rocket::Core::Context* context;

      /** True iff the Rocket context was created internally (and thus should also be destroyed internally) */
      bool internalContext;

      /** Set true to signal the state logic to terminate so that the state is destroyed. */
      bool finished;

      /**
       * Constructor.
       * Initializes the top-level GUI widget container.
       *
       * @param executionStack The execution stack that the state belongs to.
       * @param stateName The unique name of the state.
       */
      GameState(ExecutionStack& executionStack, const std::string& stateName);

      /**
       * Constructor.
       * Initializes the state with an existing top-level container.
       *
       * @param executionStack The execution stack that the state belongs to.
       * @param stateName The unique name of the state.
       * @param context The Rocket context to use for this state's GUI.
       */
      GameState(ExecutionStack& executionStack, const std::string& stateName, Rocket::Core::Context* context);

      /**
       * Runs the state's logic processing
       *
       * @return true iff the state is not finished
       */
      virtual bool step() = 0;

      /**
       * Does common event handling that is required across all game states.
       *
       * @param event The event to handle.
       */
      void handleEvent(const SDL_Event& event);

      /**
       * Runs the state's graphic and interface processing.
       * Afterwards, draws widgets, flips the buffer.
       */
      virtual void draw() = 0;

   public:
      /**
       * State activation called every time this state is found at the top of the execution stack.
       * In other words, activate is always called before this state takes control of the game loop.
       * Currently, all game state activations, at the very least, change the top level Widget container.
       */
      virtual void activate();

      /**
       * State deactivation called every time this state stops being at the top of the execution stack.
       * In other words, deactivate is always called when this state relinquishes control of the game loop.
       */
      virtual void deactivate();

      /**
       * Called every frame in order to trigger logic processing in the game state
       * that is at the top of the execution stack.
       * Generic logic that happens in every game state (such as GUI logic) should go in here.
       */
      virtual bool advanceFrame();

      /**
       * Called every frame in order to trigger drawing the game state
       * that is at the top of the execution stack.
       * Generic drawing code that is performed in every game state (such as drawing GUI and flipping the buffer) should go in here.
       */
      virtual void drawFrame();

      /**
       * Destructor.
       */
      virtual ~GameState();
};

#endif
