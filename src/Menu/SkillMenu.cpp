/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "SkillMenu.h"
#include "MenuShell.h"

#include <Rocket/Core.h>
#include <Rocket/Controls.h>

#include "GameContext.h"
#include "PlayerData.h"
#include "Skill.h"

#include "ExecutionStack.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_MENU;

SkillMenu::SkillMenu(GameContext& gameContext, PlayerData& playerData) :
   CharacterDependentMenu(gameContext, playerData, "SkillMenu"),
   bindings(this),
   playerData(playerData),
   skillViewModel(gameContext, playerData)
{
   initialize();
}

SkillMenu::SkillMenu(GameContext& gameContext, PlayerData& playerData, MenuShell* menuShell) :
   CharacterDependentMenu(gameContext, "SkillMenu", menuShell),
   bindings(this),
   playerData(playerData),
   skillViewModel(gameContext, playerData)
{
   initialize();
}

void SkillMenu::initialize()
{
   paneDocument = menuShell->getRocketContext()->LoadDocument("data/gui/skillpane.rml");
   if(paneDocument != NULL)
   {
      bindings.bindAction(paneDocument, "skillGrid", "click", &SkillMenu::skillClicked);
      bindings.bindAction(paneDocument, "skillGrid", "dragstart", &SkillMenu::dragStarted);
   }
}

SkillMenu::~SkillMenu()
{
   if(paneDocument != NULL)
   {
      paneDocument->Close();
      paneDocument->RemoveReference();
   }
}

void SkillMenu::activate()
{
   MenuState::activate();
   if(paneDocument != NULL)
   {
      paneDocument->Show();
   }
}

void SkillMenu::deactivate()
{
   if(paneDocument != NULL)
   {
      paneDocument->Hide();
   }
}

void SkillMenu::setCharacter(int characterIndex)
{
   skillViewModel.setCharacter(characterIndex);
}

void SkillMenu::dragStarted(Rocket::Core::Event* event)
{
   Rocket::Core::Element* dragElement = static_cast<Rocket::Core::Element*>(event->GetParameter< void* >("drag_element", NULL));
   if (dragElement != NULL)
   {
      Rocket::Core::Element* target = event->GetTargetElement();

      // Move up the DOM to the datagridrow skill holding this element
      while(target->GetParentNode() != NULL && target->GetTagName() != "datagridrow")
      {
         target = target->GetParentNode();
      }

      if(target != NULL)
      {
         // If we found a row element, cast it and get its index
         Rocket::Controls::ElementDataGridRow* rowElement = dynamic_cast<Rocket::Controls::ElementDataGridRow*>(target);
         if(rowElement != NULL)
         {
            const int skillIndex = rowElement->GetParentRelativeIndex();
            const UsableId skillId = skillViewModel.getSkillId(skillIndex);
            const std::string characterId = skillViewModel.getCurrentCharacterId();

            Rocket::Core::ElementAttributes dragAttributes;
            dragAttributes.Set("skillId", static_cast<int>(skillId));
            dragAttributes.Set("characterId", characterId.c_str());
            dragElement->SetAttributes(&dragAttributes);

            DEBUG("Dragging skill %d.", skillId);
         }
      }
   }
}

void SkillMenu::skillClicked(Rocket::Core::Event* event)
{
   Rocket::Core::Element* target = event->GetTargetElement();

   // Move up the DOM to the datagridrow skill holding this element
   while(target->GetParentNode() != NULL && target->GetTagName() != "datagridrow")
   {
      target = target->GetParentNode();
   }

   if(target != NULL)
   {
      // If we found a row element, cast it and get its index
      Rocket::Controls::ElementDataGridRow* rowElement = dynamic_cast<Rocket::Controls::ElementDataGridRow*>(target);
      if(rowElement != NULL)
      {
         int skillIndex = rowElement->GetParentRelativeIndex();
         skillViewModel.useSkill(skillIndex);
      }
   }
}
