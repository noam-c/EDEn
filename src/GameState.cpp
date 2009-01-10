#include "GameState.h"
#include "GraphicsUtil.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_GAME_STATE;

void GameState::activate()
{   GraphicsUtil::getInstance()->setInterface(top);
    finished = false;
}

bool GameState::step()
{   GraphicsUtil::getInstance()->stepGUI();

    return true;
}

void GameState::draw()
{   GraphicsUtil::getInstance()->drawGUI();
}
