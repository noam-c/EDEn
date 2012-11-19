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

GameState::GameState(ExecutionStack& executionStack, const std::string& stateName) : executionStack(executionStack)
{
   context = GraphicsUtil::getInstance()->createRocketContext(stateName.c_str());
}

GameState::GameState(ExecutionStack& executionStack, const std::string& stateName, Rocket::Core::Context* context) : executionStack(executionStack), context(context)
{
   context->AddReference();
}

GameState::~GameState()
{
   context->RemoveReference();
}

void GameState::activate()
{
   finished = false;
   context->Update();
}

void GameState::deactivate()
{
}

bool GameState::advanceFrame()
{
   context->Update();
   return step();
}

void GameState::handleEvent(const SDL_Event& event)
{
   RocketSDLInputMapping::handleSDLEvent(context, event);
}

void GameState::drawFrame()
{
   draw();

   context->Render();

   // Make sure everything is displayed on screen
   GraphicsUtil::getInstance()->flipScreen();
}
