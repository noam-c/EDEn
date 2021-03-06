/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "TransitionState.h"

#include "Transition.h"

#include "DebugUtils.h"
#define DEBUG_FLAG DEBUG_TRANSITIONS

TransitionState::TransitionState(GameContext& gameContext, std::unique_ptr<Transition>&& transition) :
   GameState(gameContext, GameStateType::TRANSITION, "Transition"),
   m_transition(std::move(transition))
{
}

TransitionState::~TransitionState() = default;

void TransitionState::draw()
{
   m_transition->draw();
}

bool TransitionState::step(long timePassed)
{
   return m_transition->step(timePassed);
}
