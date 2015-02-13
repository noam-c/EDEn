/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include "GameState.h"

#include "GameContext.h"
#include "GraphicsUtil.h"

#include <algorithm>

#include <SDL.h>

#include <Rocket/Core.h>
#include "RocketSDLInputMapping.h"

#include "DebugUtils.h"
#define DEBUG_FLAG DEBUG_GAME_STATE

const int GameState::MAX_FRAME_TIME = 32;

GameState::GameState(GameContext& gameContext, GameStateType stateType, const std::string& stateName) :
   m_time(SDL_GetTicks()),
   m_active(false),
   m_stateType(stateType),
   m_gameContext(gameContext),
   m_finished(false)
{
   m_rocketContext = GraphicsUtil::getInstance()->createRocketContext(stateName.c_str());
}

GameState::GameState(GameContext& gameContext, GameStateType stateType, const std::string& stateName, Rocket::Core::Context* context) :
   m_stateType(stateType),
   m_gameContext(gameContext),
   m_rocketContext(context),
   m_finished(false)
{
   context->AddReference();
}

GameState::~GameState()
{
   m_rocketContext->RemoveReference();
}

ExecutionStack* GameState::getExecutionStack() const
{
   return m_executionStack;
}

void GameState::setExecutionStack(ExecutionStack* executionStack) noexcept
{
   m_executionStack = executionStack;
}

Metadata& GameState::getMetadata() const
{
   return m_gameContext.getMetadata();
}

ScriptEngine& GameState::getScriptEngine() const
{
   return m_gameContext.getScriptEngine();
}

void GameState::activate()
{
   m_rocketContext->Update();
   m_active = true;
}

void GameState::deactivate()
{
   m_active = false;
}

bool GameState::isActive() const
{
   return m_active;
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

Scheduler* GameState::getScheduler()
{
   return nullptr;
}

GameStateType GameState::getStateType() const
{
   return m_stateType;
}
