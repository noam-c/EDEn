/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "ShortcutBar.h"
#include <string>
#include <Rocket/Core.h>
#include <Rocket/Controls.h>

#include "GameContext.h"
#include "PlayerData.h"
#include "Item.h"

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
         bindings.bindAction(shortcutContainer, "dragdrop", &ShortcutBar::itemDropped);
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

      int itemId = playerData.getShortcut(shortcutIndex);
      Item* item = gameContext.getItem(itemId);

      if(item != NULL)
      {
         item->use(gameContext);
         refresh();
      }
   }
}

void ShortcutBar::itemDropped(Rocket::Core::Event* event)
{
   Rocket::Core::Element* dragElement = static_cast<Rocket::Core::Element*>(event->GetParameter< void* >("drag_element", NULL));
   if (dragElement != NULL)
   {
      int itemId = dragElement->GetAttribute<int>("itemId", 0);
      if(itemId > 0)
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

            DEBUG("Dropping item %d on shortcut index %d.", itemId, dropTargetIndex);
            playerData.setShortcut(dropTargetIndex, itemId);

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
      int itemId = playerData.getShortcut(i);
      const Item* item = gameContext.getItem(itemId);

      Rocket::Core::Element* shortcutElement = shortcutBarDocument->CreateElement("div");
      shortcutElement->SetAttribute("class", "shortcut");

      if(item != NULL)
      {
         DEBUG("Adding shortcut for item %d", itemId);

         shortcutElement->SetAttribute("itemId", itemId);
         Rocket::Core::String shortcutIconPath("../../");
         shortcutIconPath += item->getIconPath().c_str();
         const Rocket::Core::String shortcutQuantity(8, "%d", playerData.getInventory()->getItemQuantity(itemId));

         Rocket::Core::Element* shortcutIconElement = shortcutBarDocument->CreateElement("img");
         shortcutIconElement->SetAttribute("src", shortcutIconPath);
         shortcutIconElement->SetAttribute("class", "shortcutIcon");

         Rocket::Core::Element* shortcutQuantityElement = shortcutBarDocument->CreateElement("span");
         shortcutQuantityElement->SetInnerRML(shortcutQuantity);
         shortcutQuantityElement->SetAttribute("class", "shortcutQuantity");

         shortcutElement->AppendChild(shortcutIconElement);
         shortcutElement->AppendChild(shortcutQuantityElement);
      }

      shortcutContainer->AppendChild(shortcutElement);
   }
}

int ShortcutBar::getHeight() const
{
   return shortcutContainer->GetScrollHeight();
}
