#include "ExecutionStack.h"
#include "DebugUtils.h"
#include "GameState.h"

const int debugFlag = DEBUG_EXEC_STACK;

ExecutionStack* ExecutionStack::instance = NULL;

ExecutionStack::ExecutionStack()
{   currentState = NULL;
}

ExecutionStack::~ExecutionStack()
{   while(popState());
    instance = NULL;
}

ExecutionStack* ExecutionStack::getInstance()
{   if(!instance)
    {  instance = new ExecutionStack();  }

    return instance;
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
