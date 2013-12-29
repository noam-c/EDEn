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
#include "SkillList.h"
#include "Character.h"

#include "ExecutionStack.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_MENU;

SkillMenu::SkillMenu(GameContext& gameContext) :
   CharacterDependentMenu(gameContext, "SkillMenu"),
   m_bindings(this),
   m_selectedCharacter(NULL),
   m_skillViewModel(*this, gameContext.getMetadata())
{
   initialize();
}

SkillMenu::SkillMenu(GameContext& gameContext, MenuShell* menuShell) :
   CharacterDependentMenu(gameContext, "SkillMenu", menuShell),
   m_bindings(this),
   m_skillViewModel(*this, gameContext.getMetadata())
{
   initialize();
}

void SkillMenu::initialize()
{
   m_paneDocument = m_menuShell->getRocketContext()->LoadDocument("data/gui/skillpane.rml");
   if(m_paneDocument != NULL)
   {
      m_bindings.bindAction(m_paneDocument, "skillGrid", "click", &SkillMenu::skillClicked);
      m_bindings.bindAction(m_paneDocument, "skillGrid", "dragstart", &SkillMenu::dragStarted);
   }
}

SkillMenu::~SkillMenu()
{
   if(m_paneDocument != NULL)
   {
      m_paneDocument->Close();
      m_paneDocument->RemoveReference();
   }
}

void SkillMenu::activate()
{
   MenuState::activate();
   if(m_paneDocument != NULL)
   {
      m_paneDocument->Show();
   }
}

void SkillMenu::deactivate()
{
   if(m_paneDocument != NULL)
   {
      m_paneDocument->Hide();
   }
}

void SkillMenu::setCharacter(int characterIndex)
{
   CharacterRoster* roster = m_gameContext.getCurrentPlayerData().getRoster();
   
   m_selectedCharacter = roster != NULL && roster->getParty().size() > characterIndex ?
      roster->getParty()[characterIndex] :
      NULL;

   m_skillViewModel.refresh();
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
            const UsableId skillId = m_skillViewModel.getSkillId(skillIndex);
            const std::string characterId = m_skillViewModel.getCurrentCharacterId();

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
         useSkill(skillIndex);
      }
   }
}

void SkillMenu::useSkill(int rowIndex)
{
   if(m_selectedCharacter == NULL)
   {
      return;
   }
   
   const SkillList& skillList = m_selectedCharacter->getSkillList();
   const UsableId skillId = skillList[rowIndex];
   Skill* skill = m_gameContext.getSkill(skillId);
   if(skill == NULL)
   {
      DEBUG("Tried to use bad skill with ID: %d.", skillId);
   }
   else
   {
      skill->use(m_gameContext.getScriptEngine(), getStateType(), m_selectedCharacter);
      m_skillViewModel.refresh(rowIndex);
   }
}

