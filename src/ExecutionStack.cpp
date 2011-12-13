/**
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#include "ExecutionStack.h"
#include "GraphicsUtil.h"
#include "DebugUtils.h"
#include "GameState.h"

const int debugFlag = DEBUG_EXEC_STACK;

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
}

void ExecutionStack::pushState(GameState* newState)
{
   stateStack.push(newState);
   newState->activate();
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
         // reactivate the state below it.
         popState();
         if(!stateStack.empty())
         {
            stateStack.top()->activate();
         }
      }
   }
}
