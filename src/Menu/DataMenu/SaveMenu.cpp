/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "SaveMenu.h"

#include <SDL.h>
#include <Rocket/Core.h>
#include <Rocket/Controls.h>

#include "PlayerData.h"
#include "PlayerDataSummary.h"

#include "dirent.h"

#include "ExecutionStack.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_MENU;

SaveMenu::SaveMenu(GameContext& gameContext, PlayerData& playerData, const SaveLocation saveLocation) :
   GameState(gameContext, GameStateType::MENU, "SaveMenu"),
   m_saveLocation(saveLocation),
   m_model(*this, getMetadata()),
   m_playerData(playerData),
   m_dataViewModel(m_model)
{
   m_titleDocument = m_rocketContext->LoadDocument("data/gui/datapane.rml");
   
   if(m_titleDocument != nullptr)
   {
      m_titleDocument->Show();
      m_bindings.bindAction(m_titleDocument, "keydown", [this](Rocket::Core::Event& event) { listKeyDown(event); });
      m_bindings.bindAction(m_titleDocument, "saveGameGrid", "click", [this](Rocket::Core::Event& event) { saveGameClicked(event); });
   }
   
   m_confirmSaveDocument = m_rocketContext->LoadDocument("data/gui/dataconfirmsave.rml");
   if(m_confirmSaveDocument != nullptr)
   {
      m_bindings.bindAction(m_confirmSaveDocument, "confirm", "click", [this](Rocket::Core::Event& event) { confirmClicked(event); });
      m_bindings.bindAction(m_confirmSaveDocument, "cancel", "click", [this](Rocket::Core::Event& event) { cancelClicked(event); });
   }
   
   m_slotToSave = -1;
}

SaveMenu::~SaveMenu()
{
   m_titleDocument->Close();
   m_titleDocument->RemoveReference();
   
   if(m_confirmSaveDocument != nullptr)
   {
      m_confirmSaveDocument->Close();
      m_confirmSaveDocument->RemoveReference();
   }
}

bool SaveMenu::step(long timePassed)
{
   if(m_finished) return false;
   
   m_scheduler.runCoroutines(timePassed);
   bool done = false;
   
   waitForInputEvent(done);
   
   /* The menu shouldn't run too fast */
   SDL_Delay (1);
   
   return !done;
}

void SaveMenu::waitForInputEvent(bool& finishState)
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
               finishState = true;
               return;
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
         finishState = true;
         return;
      }
      default:
      {
         break;
      }
   }
   
   handleEvent(event);
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
   
   Rocket::Core::Element* list = m_titleDocument->GetElementById("menu");
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
   m_slotToSave = index;
   if (m_confirmSaveDocument != nullptr)
   {
      m_confirmSaveDocument->Show(Rocket::Core::ElementDocument::MODAL);
   }
}

void SaveMenu::hideConfirmDialog()
{
   m_slotToSave = -1;
   if(m_confirmSaveDocument != nullptr)
   {
      // Need to call Show without focus flags first, to release modal focus :(
      m_confirmSaveDocument->Show();
      
      m_confirmSaveDocument->Hide();
   }
}

void SaveMenu::confirmClicked(Rocket::Core::Event& event)
{
   m_model.saveToSlot(m_playerData, m_saveLocation, m_slotToSave);
   hideConfirmDialog();
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
   m_dataViewModel.refresh(slotIndex);
}
