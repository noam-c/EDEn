#include "GameState.h"
#include "GraphicsUtil.h"
#include "Container.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_GAME_STATE;

GameState::GameState()
{
   top = new edwt::Container();
   top->setDimension(gcn::Rectangle(0, 0, 800, 600));
   top->setOpaque(false);
   top->setEnabled(true);
}

void GameState::activate()
{
   GraphicsUtil::getInstance()->setInterface(top);
   finished = false;
}

bool GameState::step()
{
   GraphicsUtil::getInstance()->stepGUI();
   return true;
}

void GameState::draw()
{
   GraphicsUtil::getInstance()->drawGUI();
}
