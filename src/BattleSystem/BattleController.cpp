/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "BattleController.h"

#include "stdlib.h"
#include <Rocket/Core.h>
#include <algorithm>
#include <utility>
#include <tuple>
#include <SDL.h>

#include "CombatActor.h"
#include "DebugCommandMessage.h"
#include "Direction.h"
#include "ScriptEngine.h"
#include "NPC.h"
#include "PlayerData.h"
#include "GraphicsUtil.h"
#include "ResourceLoader.h"
#include "Region.h"
#include "TriggerZone.h"
#include "Pathfinder.h"
#include "Rectangle.h"
#include "ExecutionStack.h"
#include "ScreenTexture.h"
#include "MapTriggerCallback.h"

#include "DebugUtils.h"
#define DEBUG_FLAG DEBUG_BATTLE_ENG

BattleController::BattleController(GameContext& gameContext, std::shared_ptr<PlayerData> playerData, std::shared_ptr<Task> battleTask) :
   GameState(gameContext, GameStateType::FIELD, "BattleController"),
   m_playerData(std::move(playerData)),
   m_battleTask(std::move(battleTask)),
   m_initialized(false),
   m_dialogue(getScriptEngine()),
   m_overlay(m_messagePipe, *m_playerData, getMetadata(), getStateType(), *m_rocketContext, m_dialogue)
{
   m_dialogue.initialize(m_scheduler, m_overlay.getDialogueBox());
}

BattleController::~BattleController()
{
   m_playerData->unbindMessagePipe();
}

Scheduler* BattleController::getScheduler()
{
   return &m_scheduler;
}

void BattleController::dialogueNarrate(const std::string& narration, const std::shared_ptr<Task>& task, const DialogueChoiceList& choices)
{
   m_dialogue.narrate(narration, task, choices);
}

void BattleController::dialogueSay(const std::string& speech, const std::shared_ptr<Task>& task, const DialogueChoiceList& choices)
{
   m_dialogue.say(speech, task, choices);
}

void BattleController::activate()
{
   GameState::activate();
   m_playerData->bindMessagePipe(&m_messagePipe);
   getScriptEngine().setBattleController(shared_from_this());
   getScriptEngine().setPlayerData(m_playerData);

   for(size_t i = 0; i < 4; ++i)
   {
      m_combatants.emplace_back(
         std::string("combatant_").append(std::to_string(i)),
         geometry::Point2D(300, static_cast<int>(100*(i+1))),
         geometry::Size(50,100),
         geometry::Direction::RIGHT);
      
      m_combatants[i].setSpritesheet("kain");
   }
   m_initialized = true;
}

void BattleController::deactivate()
{
   m_combatants.clear();
   getScriptEngine().setPlayerData(nullptr);
   getScriptEngine().setBattleController(nullptr);
   m_playerData->unbindMessagePipe();
   GameState::deactivate();
}

void BattleController::draw()
{
   for(auto& c : m_combatants)
   {
      c.draw();
   }
}

bool BattleController::step(long timePassed)
{
   bool done = false;
   m_scheduler.runCoroutines(timePassed);

   handleInputEvents(done);

   if(done && m_battleTask)
   {
      m_battleTask->complete();
   }

   return !done;
}

void BattleController::handleInputEvents(bool& finishState)
{
   SDL_Event event;

   while(SDL_PollEvent(&event))
   {
      switch (event.type)
      {
         case SDL_MOUSEBUTTONDOWN:
         {
            break;
         }
         case SDL_KEYDOWN:
         {
            switch(event.key.keysym.sym)
            {
               case SDLK_SPACE:
               {
                  if(!m_overlay.isDebugConsoleVisible())
                  {
                     if(m_dialogue.hasDialogue())
                     {
                        m_dialogue.setFastModeEnabled(true);

                        if (!event.key.repeat)
                        {
                           m_dialogue.nextLine();
                        }
                     }
                  }
                  return;
               }
               case SDLK_ESCAPE:
               {
                  finishState = true;
                  return;
               }
               case SDLK_BACKQUOTE:
               {
                  m_overlay.toggleDebugConsole();
                  return;
               }
               case SDLK_TAB:
               {
                  break;
               }
               default:
               {
                  break;
               }
            }
            break;
         }
         case SDL_KEYUP:
         {
            if(!m_overlay.isDebugConsoleVisible())
            {
               switch(event.key.keysym.sym)
               {
                  case SDLK_SPACE:
                  {
                     if(m_dialogue.hasDialogue())
                     {
                        m_dialogue.setFastModeEnabled(false);
                     }
                     return;
                  }
                  default:
                  {
                     break;
                  }
               }
            }
            break;
         }
         case SDL_QUIT:
         {
            finishState = true;
            return;
         }
         default:
         {
            break;
         }
      }

      // If the battle controller didn't consume this event, then propagate to the generic input handling
      handleEvent(event);
   }
}
