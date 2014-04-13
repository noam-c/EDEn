/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "SkillMenu.h"
#include "MenuShell.h"

#include <Rocket/Core.h>
#include <Rocket/Controls.h>

#include "Metadata.h"
#include "PlayerData.h"
#include "Skill.h"
#include "SkillList.h"
#include "Character.h"

#include "ExecutionStack.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_MENU;

SkillMenu::SkillMenu(GameContext& gameContext) :
   CharacterDependentMenu(gameContext, "SkillMenu"),
   m_selectedCharacter(nullptr),
   m_skillViewModel(*this, getMetadata())
{
   initialize();
}

SkillMenu::SkillMenu(GameContext& gameContext, std::shared_ptr<MenuShell> menuShell) :
   CharacterDependentMenu(gameContext, "SkillMenu", menuShell),
   m_skillViewModel(*this, getMetadata())
{
   initialize();
}

void SkillMenu::initialize()
{
   m_paneDocument = m_menuShell->getRocketContext()->LoadDocument("data/gui/skillpane.rml");
   if(m_paneDocument != nullptr)
   {
      m_bindings.bindAction(m_paneDocument, "skillGrid", "click", [this](Rocket::Core::Event& event) { skillClicked(event); });
      m_bindings.bindAction(m_paneDocument, "skillGrid", "dragstart", [this](Rocket::Core::Event& event) { dragStarted(event); });
   }
}

SkillMenu::~SkillMenu()
{
}

void SkillMenu::setCharacter(int characterIndex)
{
   CharacterRoster* roster = getCurrentPlayerData().getRoster();
   
   m_selectedCharacter = roster != nullptr && roster->getParty().size() > characterIndex ?
      roster->getParty()[characterIndex] :
      nullptr;

   m_skillViewModel.refresh();
}

void SkillMenu::dragStarted(Rocket::Core::Event& event)
{
   Rocket::Core::Element* dragElement = static_cast<Rocket::Core::Element*>(event.GetParameter< void* >("drag_element", nullptr));
   if (dragElement != nullptr)
   {
      Rocket::Core::Element* target = event.GetTargetElement();

      // Move up the DOM to the datagridrow skill holding this element
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

void SkillMenu::skillClicked(Rocket::Core::Event& event)
{
   Rocket::Core::Element* target = event.GetTargetElement();

   // Move up the DOM to the datagridrow skill holding this element
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
         int skillIndex = rowElement->GetParentRelativeIndex();
         useSkill(skillIndex);
      }
   }
}

void SkillMenu::useSkill(int rowIndex)
{
   if(m_selectedCharacter == nullptr)
   {
      return;
   }
   
   const SkillList& skillList = m_selectedCharacter->getSkillList();
   const UsableId skillId = skillList[rowIndex];
   Skill* skill = getMetadata().getSkill(skillId);
   if(skill == nullptr)
   {
      DEBUG("Tried to use bad skill with ID: %d.", skillId);
   }
   else
   {
      skill->use(getScriptEngine(), getStateType(), m_selectedCharacter);
      m_skillViewModel.refresh(rowIndex);
   }
}

Character* SkillMenu::getSelectedCharacter() const
{
   return m_selectedCharacter;
}

