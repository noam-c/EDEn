#include "ExecutionStack.h"
#include "LinkedListNode.h"
#include "GraphicsUtil.h"
#include "DebugUtils.h"
#include "GameState.h"

const int debugFlag = DEBUG_EXEC_STACK;

void ExecutionStack::initialize()
{
   currentState = NULL;
}

void ExecutionStack::finish()
{
   while(popState());
   instance = NULL;
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
