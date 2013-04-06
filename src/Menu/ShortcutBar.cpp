/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "ShortcutBar.h"
#include <string>
#include <Rocket/Core.h>
#include <Rocket/Controls.h>

#include "GameContext.h"
#include "PlayerData.h"
#include "Character.h"
#include "Shortcut.h"
#include "Item.h"
#include "Skill.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_MENU;

ShortcutBar::ShortcutBar(GameContext& gameContext, PlayerData& playerData, Rocket::Core::Context& rocketContext) :
   bindings(this),
   rocketContext(rocketContext),
   gameContext(gameContext),
   playerData(playerData)
{
   rocketContext.AddReference();

   shortcutBarDocument = rocketContext.LoadDocument("data/gui/shortcutBar.rml");
   if(shortcutBarDocument != NULL)
   {
      shortcutContainer = shortcutBarDocument->GetElementById("shortcutContainer");
      if (shortcutContainer != NULL)
      {
         bindings.bindAction(shortcutContainer, "dragdrop", &ShortcutBar::usableDropped);
         bindings.bindAction(shortcutContainer, "click", &ShortcutBar::shortcutClicked);
      }

      refresh();

      shortcutBarDocument->Show();
   }
}

ShortcutBar::~ShortcutBar()
{
   if(shortcutBarDocument != NULL)
   {
      shortcutBarDocument->RemoveReference();
      shortcutBarDocument->Close();
   }

   rocketContext.RemoveReference();
}

void ShortcutBar::shortcutClicked(Rocket::Core::Event* event)
{
   Rocket::Core::Element* shortcutElement = event->GetTargetElement();

   while(shortcutElement != NULL && shortcutElement->GetParentNode() != shortcutContainer)
   {
      shortcutElement = shortcutElement->GetParentNode();
   }

   // Only handle the click if it went to a shortcut
   // (direct child of the shortcut bar container)
   if (shortcutElement != NULL)
   {
      // Find the index of the shortcut
      int shortcutIndex = 0;

      for(;;)
      {
         shortcutElement = shortcutElement->GetPreviousSibling();
         if (shortcutElement == NULL) break;
         ++shortcutIndex;
      }

      bool shortcutInvoked = playerData.invokeShortcut(shortcutIndex);
      if (shortcutInvoked)
      {
         refresh();
      }
   }
}

void ShortcutBar::usableDropped(Rocket::Core::Event* event)
{
   Rocket::Core::Element* dragElement = static_cast<Rocket::Core::Element*>(event->GetParameter< void* >("drag_element", NULL));
   if (dragElement != NULL)
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
         Rocket::Core::Element* dropElement = event->GetTargetElement();

         // Only drops on direct children of the shortcut container count as
         // shortcut drops
         if(dropElement->GetParentNode() == shortcutContainer)
         {
            // Find the index of the shortcut
            int dropTargetIndex = 0;

            for(;;)
            {
               dropElement = dropElement->GetPreviousSibling();
               if (dropElement == NULL) break;
               ++dropTargetIndex;
            }

            bool isShortcutSet = false;
            if(isItem)
            {
               DEBUG("Dropping item %d on shortcut index %d.", usableId, dropTargetIndex);
               playerData.setShortcut(dropTargetIndex, usableId);
               isShortcutSet = true;
            }
            else if(isSkill)
            {
               const std::string characterId = dragElement->GetAttribute<Rocket::Core::String>("characterId", "").CString();
               Character* character = playerData.getRoster()->getCharacter(characterId);

               if(character != NULL)
               {
                  DEBUG("Dropping skill %d on shortcut index %d.", usableId, dropTargetIndex);
                  playerData.setShortcut(dropTargetIndex, usableId, characterId);
                  isShortcutSet = true;
               }
            }

            if(isShortcutSet)
            {
               // If the drag was initiated from the shortcut bar, then
               // clear out the shortcut's original slot
               if (dragElement->GetParentNode() == shortcutContainer)
               {
                  int dragTargetIndex = 0;
                  for(;;)
                  {
                     dragElement = dragElement->GetPreviousSibling();
                     if (dragElement == NULL) break;
                     ++dragTargetIndex;
                  }

                  playerData.clearShortcut(dragTargetIndex);
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
   shortcutContainer->SetInnerRML("");

   for (int i = 0; i < PlayerData::SHORTCUT_BAR_SIZE; ++i)
   {
      const Shortcut& shortcut = playerData.getShortcut(i);
      const Usable* usable =
            shortcut.usableType == Shortcut::ITEM ?
            static_cast<Usable*>(gameContext.getItem(shortcut.usableId)) :
            static_cast<Usable*>(gameContext.getSkill(shortcut.usableId));

      Rocket::Core::Element* shortcutElement = shortcutBarDocument->CreateElement("div");
      shortcutElement->SetAttribute("class", "shortcut");

      if(usable != NULL)
      {
         if (shortcut.usableType == Shortcut::ITEM)
         {
            DEBUG("Adding shortcut for item %d", usable->getId());
            shortcutElement->SetAttribute("itemId", static_cast<int>(shortcut.usableId));
         }
         else
         {
            DEBUG("Adding shortcut for skill %d", usable->getId());
            shortcutElement->SetAttribute("skillId", static_cast<int>(shortcut.usableId));
            shortcutElement->SetAttribute("characterId", shortcut.characterId.c_str());
         }

         Rocket::Core::String shortcutIconPath("../../");
         shortcutIconPath += usable->getIconPath().c_str();
         Rocket::Core::Element* shortcutIconElement = shortcutBarDocument->CreateElement("img");
         shortcutIconElement->SetAttribute("src", shortcutIconPath);
         shortcutIconElement->SetAttribute("class", "shortcutIcon");

         if (shortcut.usableType == Shortcut::ITEM)
         {
            const Rocket::Core::String shortcutQuantity(8, "%d", playerData.getInventory()->getItemQuantity(shortcut.usableId));
            Rocket::Core::Element* shortcutQuantityElement = shortcutBarDocument->CreateElement("span");
            shortcutQuantityElement->SetInnerRML(shortcutQuantity);
            shortcutQuantityElement->SetAttribute("class", "shortcutQuantity");
            shortcutElement->AppendChild(shortcutQuantityElement);
         }

         shortcutElement->AppendChild(shortcutIconElement);
      }

      shortcutContainer->AppendChild(shortcutElement);
   }
}

int ShortcutBar::getHeight() const
{
   return shortcutContainer->GetScrollHeight();
}
