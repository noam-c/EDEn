/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <memory>
#include <string>

class ExecutionStack;
class GameContext;
class Metadata;
class PlayerData;
class Scheduler;
class ScriptEngine;

namespace Rocket
{
   namespace Core
   {
      class Context;
   }
};

union SDL_Event;


enum class GameStateType
{
   UNKNOWN,
   TITLE,
   FIELD,
   MENU,
   BATTLE,
   TRANSITION,
};

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
   friend class ExecutionStack;

   private:
      /** The minimum amount of milliseconds that should pass between frames. */
      const static int MIN_FRAME_TIME;
   
      /** The maximum amount of milliseconds allowed to pass between frames. */
      const static int MAX_FRAME_TIME;

      /** Timestamp of the last logic step of the state. */
      unsigned long m_time;

      /** True iff the GameState has been activated. */
      bool m_active = false;

      /** The type of the current game state. */
      GameStateType m_stateType;

      ExecutionStack* m_executionStack;

      void setExecutionStack(ExecutionStack* executionStack) noexcept;

   protected:
      ExecutionStack* getExecutionStack() const;
      ScriptEngine& getScriptEngine() const;
      Metadata& getMetadata() const;

      /** The game context responsible for the game's data and settings. */
      GameContext& m_gameContext;

      /** The Rocket context for any GUI created by the state. */
      Rocket::Core::Context* m_rocketContext;

      /** Set true to signal the state logic to terminate so that the state is destroyed. */
      bool m_finished = false;

      /**
       * Constructor.
       * Initializes the top-level GUI widget container.
       *
       * @param gameContext The game context responsible for the state's data and execution.
       * @param stateType The type of the state.
       * @param stateName The unique name of the state.
       */
      GameState(GameContext& gameContext, GameStateType stateType, const std::string& stateName);

      /**
       * Constructor.
       * Initializes the state with an existing top-level container.
       *
       * @param gameContext The game context responsible for the state's data and execution.
       * @param stateType The type of the state.
       * @param stateName The unique name of the state.
       * @param rocketContext The Rocket context to use for this state's GUI.
       */
      GameState(GameContext& gameContext, GameStateType stateType, const std::string& stateName, Rocket::Core::Context* rocketContext);

      /**
       * Runs the state's logic processing
       *
       * @return true iff the state is not finished
       */
      virtual bool step(long timePassed) = 0;

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
       * @return true iff the GameState has been activated.
       */
      bool isActive() const;

      /**
       * Called every frame in order to trigger logic processing in the game state
       * that is at the top of the execution stack.
       * Generic logic that happens in every game state (such as GUI logic) should go in here.
       *
       * @return true iff the state is not yet finished.
       */
      bool advanceFrame();

      /**
       * Called every frame in order to trigger drawing the game state
       * that is at the top of the execution stack.
       * Generic drawing code that is performed in every game state (such as drawing GUI and flipping the buffer) should go in here.
       */
      void drawFrame();

      /**
       * @return the state's coroutine scheduler, or nullptr if none exists.
       */
      virtual Scheduler* getScheduler();

      /**
       * @return the type of this game state.
       */
      GameStateType getStateType() const;

      /**
       * Destructor.
       */
      virtual ~GameState();
};

#endif
