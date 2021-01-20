/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "ShortcutBar.h"
#include <memory>
#include <string>

#include <RmlUi/Core.h>
#include <RmlUi/Controls.h>

#include "Metadata.h"
#include "PlayerData.h"
#include "Character.h"
#include "Shortcut.h"
#include "Item.h"
#include "Skill.h"

#include "DebugUtils.h"
#define DEBUG_FLAG DEBUG_MENU

ShortcutBar::ShortcutBar(PlayerData& playerData, Metadata& metadata, GameStateType stateType, Rml::Core::Context& rocketContext) :
   m_rocketContext(rocketContext),
   m_metadata(metadata),
   m_playerData(playerData),
   m_stateType(stateType)
{
   m_shortcutBarDocument = rocketContext.LoadDocument("data/gui/shortcutBar.rml");
   if(m_shortcutBarDocument != nullptr)
   {
      m_shortcutContainer = m_shortcutBarDocument->GetElementById("shortcutContainer");
      if (m_shortcutContainer != nullptr)
      {
         m_bindings.bindAction(m_shortcutContainer, "dragdrop", [this](Rml::Core::Event& event) { usableDropped(event); });
         m_bindings.bindAction(m_shortcutContainer, "click", [this](Rml::Core::Event& event) { shortcutClicked(event); });
      }

      refresh();

      m_shortcutBarDocument->Show();
   }
}

ShortcutBar::~ShortcutBar()
{
   if(m_shortcutBarDocument != nullptr)
   {
      m_shortcutBarDocument->Close();
   }
}

void ShortcutBar::shortcutClicked(Rml::Core::Event& event)
{
   auto shortcutElement = event.GetTargetElement();

   while(shortcutElement != nullptr && shortcutElement->GetParentNode() != m_shortcutContainer)
   {
      shortcutElement = shortcutElement->GetParentNode();
   }

   // Only handle the click if it went to a shortcut
   // (direct child of the shortcut bar container)
   if (shortcutElement != nullptr)
   {
      // Find the index of the shortcut
      int shortcutIndex = 0;

      for(;;)
      {
         shortcutElement = shortcutElement->GetPreviousSibling();
         if (shortcutElement == nullptr) break;
         ++shortcutIndex;
      }

      bool shortcutInvoked = invokeShortcut(shortcutIndex);
      if (shortcutInvoked)
      {
         refresh();
      }
   }
}

bool ShortcutBar::invokeShortcut(int shortcutIndex)
{
   const Shortcut& shortcut = m_playerData.getShortcut(shortcutIndex);
   if(shortcut.usableType == Shortcut::UsableType::ITEM)
   {
      return m_metadata.useItem(shortcut.usableId, m_stateType);
   }
   else if(shortcut.usableType == Shortcut::UsableType::SKILL)
   {
      CharacterRoster* roster = m_playerData.getRoster();
      Character* usingCharacter = roster != nullptr ? roster->getCharacter(shortcut.characterId) : nullptr;
      if(usingCharacter != nullptr)
      {
         return m_metadata.useSkill(shortcut.usableId, m_stateType, usingCharacter);
      }
   }

   return false;
}

void ShortcutBar::usableDropped(Rml::Core::Event& event)
{
   auto dragElement = static_cast<Rml::Core::Element*>(event.GetParameter< void* >("drag_element", nullptr));
   if (dragElement != nullptr)
   {
      const bool isItem = dragElement->HasAttribute("itemId");
      const bool isSkill = dragElement->HasAttribute("skillId") && dragElement->HasAttribute("characterId");

      UsableId usableId = 0;

      if(isItem)
      {
         usableId = static_cast<UsableId>(dragElement->GetAttribute<int>("itemId", 0));
      }
      else if(isSkill)
      {
         usableId = static_cast<UsableId>(dragElement->GetAttribute<int>("skillId", 0));
      }

      if(usableId > 0)
      {
         auto dropElement = event.GetTargetElement();

         // Only drops on direct children of the shortcut container count as
         // shortcut drops
         if(dropElement->GetParentNode() == m_shortcutContainer)
         {
            // Find the index of the shortcut
            int dropTargetIndex = 0;

            for(;;)
            {
               dropElement = dropElement->GetPreviousSibling();
               if (dropElement == nullptr) break;
               ++dropTargetIndex;
            }

            bool isShortcutSet = false;
            if(isItem)
            {
               DEBUG("Dropping item %d on shortcut index %d.", usableId, dropTargetIndex);
               m_playerData.setShortcut(dropTargetIndex, usableId);
               isShortcutSet = true;
            }
            else if(isSkill)
            {
               const std::string characterId = dragElement->GetAttribute<Rml::Core::String>("characterId", "").c_str();
               Character* character = m_playerData.getRoster()->getCharacter(characterId);

               if(character != nullptr)
               {
                  DEBUG("Dropping skill %d on shortcut index %d.", usableId, dropTargetIndex);
                  m_playerData.setShortcut(dropTargetIndex, usableId, characterId);
                  isShortcutSet = true;
               }
            }

            if(isShortcutSet)
            {
               // If the drag was initiated from the shortcut bar, then
               // clear out the shortcut's original slot
               if (dragElement->GetParentNode() == m_shortcutContainer)
               {
                  int dragTargetIndex = 0;
                  for(;;)
                  {
                     dragElement = dragElement->GetPreviousSibling();
                     if (dragElement == nullptr) break;
                     ++dragTargetIndex;
                  }

                  m_playerData.clearShortcut(dragTargetIndex);
               }
            }

            refresh();
         }
      }
   }
}

void ShortcutBar::refresh()
{
   DEBUG("Refreshing shortcut bar...");
   m_shortcutContainer->SetInnerRML("");

   for (int i = 0; i < PlayerData::SHORTCUT_BAR_SIZE; ++i)
   {
      const Shortcut& shortcut = m_playerData.getShortcut(i);
      const Usable* usable =
         shortcut.usableType == Shortcut::UsableType::ITEM ?
            static_cast<const Usable*>(m_metadata.getItem(shortcut.usableId)) :
            static_cast<const Usable*>(m_metadata.getSkill(shortcut.usableId));

      auto shortcutElement = m_shortcutBarDocument->CreateElement("div");
      Rml::Core::ElementAttributes shortcutElementAttributes;
      
      shortcutElementAttributes.emplace("class", "shortcut");

      if(usable != nullptr)
      {
         if (shortcut.usableType == Shortcut::UsableType::ITEM)
         {
            DEBUG("Adding shortcut for item %d", shortcut.usableId);
            shortcutElementAttributes.emplace("itemId", static_cast<int>(shortcut.usableId));
         }
         else
         {
            DEBUG("Adding shortcut for skill %d", shortcut.usableId);
            shortcutElementAttributes.emplace("skillId", static_cast<int>(shortcut.usableId));
            shortcutElementAttributes.emplace("characterId", shortcut.characterId.c_str());
         }

         Rml::Core::String shortcutIconPath("../");
         shortcutIconPath += usable->getIconPath().c_str();
         auto shortcutIconElement = m_shortcutBarDocument->CreateElement("img");

         Rml::Core::ElementAttributes shortcutIconElementAttributes;
         shortcutIconElementAttributes.emplace("src", shortcutIconPath);
         shortcutIconElementAttributes.emplace("class", "shortcutIcon");

         if (shortcut.usableType == Shortcut::UsableType::ITEM)
         {
            const Rml::Core::String shortcutQuantity(std::to_string(m_playerData.getInventory()->getItemQuantity(shortcut.usableId)));
            auto shortcutQuantityElement = m_shortcutBarDocument->CreateElement("span");

            shortcutQuantityElement->SetInnerRML(shortcutQuantity);
            shortcutQuantityElement->SetAttribute("class", "shortcutQuantity");
            shortcutElement->AppendChild(std::move(shortcutQuantityElement));
         }

         shortcutIconElement->SetAttributes(shortcutIconElementAttributes);
         shortcutElement->AppendChild(std::move(shortcutIconElement));
      }

      shortcutElement->SetAttributes(shortcutElementAttributes);
      m_shortcutContainer->AppendChild(std::move(shortcutElement));
   }
}

int ShortcutBar::getHeight() const
{
   return m_shortcutContainer->GetOffsetHeight();
}
