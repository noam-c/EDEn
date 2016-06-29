/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "ExecutionStack.h"
#include "GraphicsUtil.h"
#include "DebugUtils.h"
#include "GameState.h"

#define DEBUG_FLAG DEBUG_EXEC_STACK

void ExecutionStack::popState()
{
   std::shared_ptr<GameState> topState = m_stateStack.top();
   m_stateStack.pop();

   topState->deactivate();

   if(m_nextState)
   {
      pushState(m_nextState);
      m_nextState.reset();
   }
}

std::shared_ptr<GameState>& ExecutionStack::getCurrentState()
{
   return m_stateStack.top();
}

const std::shared_ptr<GameState>& ExecutionStack::getCurrentState() const
{
   return m_stateStack.top();
}

Scheduler* ExecutionStack::getCurrentScheduler() const
{
   const std::shared_ptr<GameState>& currentState = getCurrentState();
   if(currentState != nullptr)
   {
      return currentState->getScheduler();
   }

   return nullptr;
}

void ExecutionStack::pushState(std::shared_ptr<GameState> newState, std::shared_ptr<GameState>
transitionState)
{
   if(m_nextState && transitionState) {
      DEBUG("Attempted to push a transition in the middle of a transition. Aborting new transition.");
      transitionState.reset();
   }

   auto stateToDeactivate = !m_stateStack.empty() ?
      m_stateStack.top() :
      nullptr;

   auto& stateToPush = transitionState ? transitionState : newState;

   m_stateStack.push(stateToPush);
   stateToPush->setExecutionStack(this);

   if(transitionState)
   {
      m_nextState = newState;
      newState->setExecutionStack(this);
   }

   if(stateToDeactivate)
   {
      stateToDeactivate->deactivate();
   }

   stateToPush->activate();
}

void ExecutionStack::execute()
{
   while(!m_stateStack.empty())
   {
      std::shared_ptr<GameState>& currentState = m_stateStack.top();
      if(currentState->advanceFrame())
      {
         // The state is still active, so draw its results
         GraphicsUtil::getInstance()->clearBuffer();
         currentState->drawFrame();
      }
      else
      {
         // Delete the current state if it is finished, then
         // reactivate the next state atop the stack.
         popState();
         if(!m_stateStack.empty())
         {
            m_stateStack.top()->activate();
         }
      }
   }
}
