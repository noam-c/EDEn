#ifndef RANDOM_TRANSITION_H
#define RANDOM_TRANSITION_H

class ExecutionStack;
class GameState;

class RandomTransition
{
  public:
    static GameState* create(ExecutionStack& executionStack, GameState* oldState, GameState* newState, long transitionLength = 1000);
};

#endif