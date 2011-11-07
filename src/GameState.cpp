#include "GameState.h"
#include "GraphicsUtil.h"
#include <SDL.h>
#include "Container.h"
#include "DebugConsoleWindow.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_GAME_STATE;

GameState::GameState(ExecutionStack& executionStack) : executionStack(executionStack), internalContainer(true)
{
   top = new edwt::Container();
   top->setDimension(gcn::Rectangle(0, 0, GraphicsUtil::getInstance()->getWidth(), GraphicsUtil::getInstance()->getHeight()));
   top->setOpaque(false);
   top->setEnabled(true);
}

GameState::GameState(ExecutionStack& executionStack, edwt::Container* container) : executionStack(executionStack), top(container), internalContainer(false)
{
}

void GameState::activate()
{
   GraphicsUtil::getInstance()->setInterface(top);
   finished = false;
}

bool GameState::advanceFrame()
{
   GraphicsUtil::getInstance()->stepGUI();
   return step();
}

void GameState::handleEvent(SDL_Event& event)
{
   GraphicsUtil::getInstance()->pushInput(event);
}

void GameState::drawFrame()
{
   draw();

   GraphicsUtil::getInstance()->drawGUI();

   // Make sure everything is displayed on screen
   GraphicsUtil::getInstance()->flipScreen();
}

GameState::~GameState()
{
   if(internalContainer)
   {
      GraphicsUtil::getInstance()->setInterface(NULL);
      delete top;
   }
}