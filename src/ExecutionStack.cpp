/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "ExecutionStack.h"
#include "GraphicsUtil.h"
#include "DebugUtils.h"
#include "GameState.h"

const int debugFlag = DEBUG_EXEC_STACK;

ExecutionStack::ExecutionStack() : nextState(NULL)
{
}

ExecutionStack::~ExecutionStack()
{
   // Delete all states on the stack
   while(!stateStack.empty())
   {
      popState();
   }
}

void ExecutionStack::popState()
{
   GameState* topState = stateStack.top();
   stateStack.pop();
   delete topState;

   if(nextState)
   {
      pushState(nextState);
      nextState = NULL;
   }
}

void ExecutionStack::pushState(GameState* newState, GameState* transitionState)
{
   if(transitionState)
   {
      stateStack.push(transitionState);
      if(nextState)
      {
         T_T("Attempted to push a transition in the middle of a transition.");
      }

      nextState = newState;
   }
   else
   {
      stateStack.push(newState);
   }

   stateStack.top()->activate();
}

void ExecutionStack::execute()
{
   while(!stateStack.empty())
   {
      GameState* currentState = stateStack.top();
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
         if(!stateStack.empty())
         {
            stateStack.top()->activate();
         }
      }
   }
}
