#include "ExecutionStack.h"
#include "DebugUtils.h"
#include "GameState.h"

const int debugFlag = DEBUG_EXEC_STACK;

void ExecutionStack::initialize()
{   currentState = NULL;
}

void ExecutionStack::finish()
{   while(popState());
    instance = NULL;
}

void ExecutionStack::pushState(GameState* newState)
{   StateNode* node = new StateNode(newState, currentState);
    currentState = node;
    newState->activate();
}

bool ExecutionStack::popState()
{   if(currentState)
    {  StateNode* oldState = currentState;
       currentState = currentState->prevState;

       delete oldState;
    }

    return currentState;
}

void ExecutionStack::execute()
{   while(currentState)
    {   if(currentState->state->step())
        {  currentState->state->draw();  }
        else
        {  if(popState())
           {  currentState->state->activate();
           }
        }
    }
}
