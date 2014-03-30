/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "ExecutionStack.h"
#include "GraphicsUtil.h"
#include "DebugUtils.h"
#include "GameState.h"

const int debugFlag = DEBUG_EXEC_STACK;

ExecutionStack::ExecutionStack() :
   m_nextState(nullptr)
{
}

ExecutionStack::~ExecutionStack()
{
   // Delete all states on the stack
   while(!m_stateStack.empty())
   {
      popState();
   }
}

void ExecutionStack::popState()
{
   GameState* topState = m_stateStack.top();
   topState->deactivate();
   m_stateStack.pop();
   delete topState;

   if(m_nextState)
   {
      pushState(m_nextState);
      m_nextState = nullptr;
   }
}

GameState* ExecutionStack::getCurrentState() const
{
   return m_stateStack.top();
}

Scheduler* ExecutionStack::getCurrentScheduler() const
{
   GameState* currentState = getCurrentState();
   if(currentState != nullptr)
   {
      return currentState->getScheduler();
   }
   
   return nullptr;
}


void ExecutionStack::pushState(GameState* newState, GameState* transitionState)
{
   if(!m_stateStack.empty())
   {
      m_stateStack.top()->deactivate();
   }
   
   newState->setExecutionStack(this);

   if(transitionState)
   {
      transitionState->setExecutionStack(this);

      m_stateStack.push(transitionState);
      if(m_nextState)
      {
         T_T("Attempted to push a transition in the middle of a transition.");
      }

      m_nextState = newState;
   }
   else
   {
      m_stateStack.push(newState);
   }

   m_stateStack.top()->activate();
}

void ExecutionStack::execute()
{
   while(!m_stateStack.empty())
   {
      GameState* currentState = m_stateStack.top();
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
