/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef SHORTCUT_BAR_H
#define SHORTCUT_BAR_H

#include "EdenRocketBindings.h"

namespace Rocket
{
   namespace Core
   {
      class Context;
      class ElementDocument;
   };
};

class GameContext;
class PlayerData;

class ShortcutBar
{
   /** The event binding collection for the debug window */
   EdenRocketBindings<ShortcutBar> bindings;

   /** The Rocket context that holds this console window */
   Rocket::Core::Context& rocketContext;

   /** The game context containing the item metadata */
   GameContext& gameContext;

   /** The Player data containing the shortcut set */
   PlayerData& playerData;

   /** The Rocket document representing the shortcut bar GUI */
   Rocket::Core::ElementDocument* shortcutBarDocument;

   /** The Rocket element that contains the shortcut bar icons */
   Rocket::Core::Element* shortcutContainer;

   public:
      /**
       * Constructor.
       *
       * @param rocketContext The Rocket context to which this console window should attach.
       */
      ShortcutBar(GameContext& gameContext, PlayerData& playerData, Rocket::Core::Context& rocketContext);

      /**
       * Destructor.
       */
      ~ShortcutBar();

      /**
       *
       */
      void itemDropped(Rocket::Core::Event* event);

      /**
       * Refresh the shortcut bar UI.
       */
      void refresh();

      /**
       * @return the total height of the shortcut bar.
       */
      int getHeight() const;
};

#endif
