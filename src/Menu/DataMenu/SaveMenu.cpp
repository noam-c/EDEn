/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include "SaveMenu.h"

#include <SDL.h>
#include <Rocket/Core.h>
#include <Rocket/Controls.h>

#include "PlayerData.h"
#include "PlayerDataSummary.h"

#include "ExecutionStack.h"
#include "TileEngine.h"
#include "TransitionState.h"
#include "FadeTransition.h"
#include "ScreenTexture.h"
#include "DebugUtils.h"

#define DEBUG_FLAG DEBUG_MENU

SaveMenu::SaveMenu(GameContext& gameContext) :
   GameState(gameContext, GameStateType::MENU, "LoadMenu"),
   m_model(*this, getMetadata()),
   m_saveLocation(SaveLocation()),
   m_saveGameViewModel(m_model),
   m_selectedSlot(-1)
{
   initializeMenu();
   initializeConfirmDialog(true /*loadMode*/);
}

SaveMenu::SaveMenu(GameContext& gameContext, std::weak_ptr<PlayerData> playerData, const SaveLocation saveLocation) :
   GameState(gameContext, GameStateType::MENU, "SaveMenu"),
   m_model(*this, getMetadata()),
   m_saveLocation(saveLocation),
   m_playerData(playerData),
   m_saveGameViewModel(m_model),
   m_selectedSlot(-1)
{
   initializeMenu();
   initializeConfirmDialog(false /*loadMode*/);
}

SaveMenu::~SaveMenu()
{
   m_menuDocument->Close();
   m_menuDocument->RemoveReference();

   if(m_confirmDocument != nullptr)
   {
      m_confirmDocument->Close();
      m_confirmDocument->RemoveReference();
   }
}

void SaveMenu::initializeMenu()
{
   m_menuDocument = m_rocketContext->LoadDocument("data/gui/datapane.rml");

   if(m_menuDocument != nullptr)
   {
      m_menuDocument->Show();
      m_bindings.bindAction(m_menuDocument, "keydown", [this](Rocket::Core::Event& event) { listKeyDown(event); });
      m_bindings.bindAction(m_menuDocument, "saveGameGrid", "click", [this](Rocket::Core::Event& event) { saveGameClicked(event); });
   }
}

void SaveMenu::initializeConfirmDialog(bool loadMode)
{
   auto documentPath = loadMode ?
      "data/gui/dataconfirmload.rml" :
      "data/gui/dataconfirmsave.rml";

   m_confirmDocument = m_rocketContext->LoadDocument(documentPath);
   if(m_confirmDocument != nullptr)
   {
      std::function<void(Rocket::Core::Event&)> confirmAction;
      if(loadMode)
      {
         confirmAction = [this](Rocket::Core::Event& event) { confirmLoadClicked(event); };
      }
      else
      {
         confirmAction = [this](Rocket::Core::Event& event) { confirmSaveClicked(event); };
      }

      m_bindings.bindAction(m_confirmDocument, "confirm", "click", confirmAction);
      m_bindings.bindAction(m_confirmDocument, "cancel", "click", [this](Rocket::Core::Event& event) { cancelClicked(event); });
   }
}

void SaveMenu::activate()
{
   // By default, GameState resets the
   // finish flag of a state when it is
   // reactivated. In this case, we actually
   // want to stay finished, since Load Game should
   // not be resumed after a game is loaded.
   if(!m_finished)
   {
      GameState::activate();
   }
}

bool SaveMenu::step(long timePassed)
{
   if(m_finished) return false;

   m_scheduler.runCoroutines(timePassed);
   bool done = waitForInputEvent();

   /* The menu shouldn't run too fast */
   SDL_Delay (1);

   return !done;
}

bool SaveMenu::waitForInputEvent()
{
   SDL_Delay (1);

   SDL_Event event;

   /* Check for events */
   SDL_PollEvent(&event);

   switch (event.type)
   {
      case SDL_KEYDOWN:
      {
         switch(event.key.keysym.sym)
         {
            case SDLK_ESCAPE:
            {
               return true;
            }
            default:
            {
               break;
            }
         }

         break;
      }
      case SDL_QUIT:
      {
         return true;
      }
      default:
      {
         break;
      }
   }

   handleEvent(event);
   return false;
}

void SaveMenu::listKeyDown(Rocket::Core::Event& event)
{
   Rocket::Core::Input::KeyIdentifier key = static_cast<Rocket::Core::Input::KeyIdentifier>(event.GetParameter<int>("key_identifier", Rocket::Core::Input::KI_UNKNOWN));

   switch(key)
   {
      case Rocket::Core::Input::KI_UP:
      case Rocket::Core::Input::KI_DOWN:
      case Rocket::Core::Input::KI_RETURN:
         break;
      default:
         return;
   }

   Rocket::Core::Element* list = m_menuDocument->GetElementById("menu");
   if(list == nullptr)
   {
      return;
   }

   Rocket::Core::Element* child = list->GetFirstChild();
   while(child != nullptr)
   {
      if(child->IsClassSet("selected"))
      {
         break;
      }

      child = child->GetNextSibling();
   }

   if(child == nullptr)
   {
      return;
   }

   if(key == Rocket::Core::Input::KI_RETURN)
   {
      child->Click();
   }
   else if(key == Rocket::Core::Input::KI_UP)
   {
      Rocket::Core::Element* previousSibling = child->GetPreviousSibling();
      if(previousSibling != nullptr)
      {
         child->SetClass("selected", false /*activate*/);
         previousSibling->SetClass("selected", true /*activate*/);
      }
   }
   else if(key == Rocket::Core::Input::KI_DOWN)
   {
      Rocket::Core::Element* nextSibling = child->GetNextSibling();
      if(nextSibling != nullptr)
      {
         child->SetClass("selected", false /*activate*/);
         nextSibling->SetClass("selected", true /*activate*/);
      }
   }
}

void SaveMenu::draw()
{
}

Scheduler* SaveMenu::getScheduler()
{
   return &m_scheduler;
}

void SaveMenu::showConfirmDialog(int index)
{
   m_selectedSlot = index;
   if (m_confirmDocument != nullptr)
   {
      m_confirmDocument->Show(Rocket::Core::ElementDocument::MODAL);
   }
}

void SaveMenu::hideConfirmDialog()
{
   m_selectedSlot = -1;
   if(m_confirmDocument != nullptr)
   {
      // Need to call Show without focus flags first, to release modal focus :(
      m_confirmDocument->Show();

      m_confirmDocument->Hide();
   }
}

void SaveMenu::confirmSaveClicked(Rocket::Core::Event& event)
{
   auto playerData = m_playerData.lock();
   if(playerData)
   {
      m_model.saveToSlot(*playerData, m_saveLocation, m_selectedSlot);
      hideConfirmDialog();
   }
}

void SaveMenu::confirmLoadClicked(Rocket::Core::Event& event)
{
   auto loadResult = m_model.loadSaveGame(m_selectedSlot);
   hideConfirmDialog();
   auto tileEngine = std::make_shared<TileEngine>(m_gameContext, std::get<0>(loadResult), std::get<1>(loadResult));
   auto fadeTransition = std::make_shared<FadeTransition>(ScreenTexture::create(*this));

   getExecutionStack()->pushState(tileEngine, std::make_shared<TransitionState>(m_gameContext, fadeTransition));
   m_finished = true;
}

void SaveMenu::cancelClicked(Rocket::Core::Event& event)
{
   hideConfirmDialog();
}

void SaveMenu::saveGameClicked(Rocket::Core::Event& event)
{
   Rocket::Core::Element* target = event.GetTargetElement();

   // Move up the DOM to the datagridrow item holding this element
   while(target->GetParentNode() != nullptr && target->GetTagName() != "datagridrow")
   {
      target = target->GetParentNode();
   }

   if(target != nullptr)
   {
      // If we found a row element, cast it and get its index
      Rocket::Controls::ElementDataGridRow* rowElement = dynamic_cast<Rocket::Controls::ElementDataGridRow*>(target);
      if(rowElement != nullptr)
      {
         int saveGameIndex = rowElement->GetParentRelativeIndex();
         showConfirmDialog(saveGameIndex);
      }
   }
}

void SaveMenu::refresh(int slotIndex)
{
   m_saveGameViewModel.refresh(slotIndex);
}
