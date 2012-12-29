/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "GameState.h"
#include "GraphicsUtil.h"
#include <Rocket/Core.h>
#include "RocketSDLInputMapping.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_GAME_STATE;

GameState::GameState(GameContext& gameContext, const std::string& stateName) :
   gameContext(gameContext)
{
   rocketContext = GraphicsUtil::getInstance()->createRocketContext(stateName.c_str());
}

GameState::GameState(GameContext& gameContext, const std::string& stateName, Rocket::Core::Context* context) :
   gameContext(gameContext),
   rocketContext(context)
{
   context->AddReference();
}

GameState::~GameState()
{
   rocketContext->RemoveReference();
}

void GameState::activate()
{
   finished = false;
   rocketContext->Update();
}

void GameState::deactivate()
{
}

bool GameState::advanceFrame()
{
   rocketContext->Update();
   return step();
}

void GameState::handleEvent(const SDL_Event& event)
{
   RocketSDLInputMapping::handleSDLEvent(rocketContext, event);
}

void GameState::drawFrame()
{
   draw();

   rocketContext->Render();

   // Make sure everything is displayed on screen
   GraphicsUtil::getInstance()->flipScreen();
}

Scheduler* GameState::getScheduler() const
{
   return NULL;
}
