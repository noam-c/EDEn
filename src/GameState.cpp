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
   m_time(SDL_GetTicks()),
   m_stateType(stateType),
   m_gameContext(gameContext)
{
   m_rocketContext = GraphicsUtil::getInstance()->createRocketContext(stateName.c_str());
}

GameState::GameState(GameContext& gameContext, GameStateType stateType, const std::string& stateName, Rocket::Core::Context* context) :
   m_stateType(stateType),
   m_gameContext(gameContext),
   m_rocketContext(context)
{
   context->AddReference();
}

GameState::~GameState()
{
   m_rocketContext->RemoveReference();
}

void GameState::activate()
{
   m_finished = false;
   m_rocketContext->Update();
}

void GameState::deactivate()
{
}

bool GameState::advanceFrame()
{
   m_rocketContext->Update();

   long prevTime = m_time;
   m_time = SDL_GetTicks();

   long timePassed = std::min<long>(m_time - prevTime, GameState::MAX_FRAME_TIME);

   return step(timePassed);
}

void GameState::handleEvent(const SDL_Event& event)
{
   RocketSDLInputMapping::handleSDLEvent(m_rocketContext, event);
}

void GameState::drawFrame()
{
   draw();

   m_rocketContext->Render();

   // Make sure everything is displayed on screen
   GraphicsUtil::getInstance()->flipScreen();
}

Scheduler* GameState::getScheduler() const
{
   return NULL;
}

GameState::GameStateType GameState::getStateType() const
{
   return m_stateType;
}
