/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "GameState.h"
#include "GraphicsUtil.h"

#include <SDL.h>

#include <Rocket/Core.h>
#include "RocketSDLInputMapping.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_GAME_STATE;

const int GameState::MAX_FRAME_TIME = 32;

GameState::GameState(GameContext& gameContext, GameStateType stateType, const std::string& stateName) :
   time(SDL_GetTicks()),
   stateType(stateType),
   gameContext(gameContext)
{
   rocketContext = GraphicsUtil::getInstance()->createRocketContext(stateName.c_str());
}

GameState::GameState(GameContext& gameContext, GameStateType stateType, const std::string& stateName, Rocket::Core::Context* context) :
   stateType(stateType),
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

   long prevTime = time;
   time = SDL_GetTicks();

   long timePassed = std::min<long>(time - prevTime, GameState::MAX_FRAME_TIME);

   return step(timePassed);
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

GameState::GameStateType GameState::getStateType() const
{
   return stateType;
}
