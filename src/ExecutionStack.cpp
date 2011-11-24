/**
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#include "ExecutionStack.h"
#include "LinkedListNode.h"
#include "GraphicsUtil.h"
#include "DebugUtils.h"
#include "GameState.h"

const int debugFlag = DEBUG_EXEC_STACK;

ExecutionStack::ExecutionStack() : currentState(NULL)
{
}

ExecutionStack::~ExecutionStack()
{
   while(popState());
}

void ExecutionStack::pushState(GameState* newState)
{
   StateNode* node = new StateNode(newState, currentState);
   currentState = node;
   newState->activate();
}

bool ExecutionStack::popState()
{
   if(currentState)
   {
      StateNode* oldState = currentState;
      currentState = currentState->next;

      /** 
       *  \todo This might be kind of a hacky solution.
       *  Maybe encapsulate this in LinkedListNode if it's going to actually happen?
       */
      oldState->next = NULL;

      delete oldState->data;
      delete oldState;
   }

   return currentState;
}

void ExecutionStack::execute()
{
   while(currentState)
   {
      if(currentState->data->advanceFrame())
      {
         GraphicsUtil::getInstance()->clearBuffer();
         currentState->data->drawFrame();
      }
      else
      {
         if(popState())
         {
            currentState->data->activate();
         }
      }
   }
}
