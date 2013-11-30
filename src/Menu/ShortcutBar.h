/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
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

class ShortcutBar
{
   /** The event binding collection for the debug window */
   EdenRocketBindings<ShortcutBar> m_bindings;

   /** The Rocket context that holds this console window */
   Rocket::Core::Context& m_rocketContext;

   /** The game context containing the item metadata */
   GameContext& m_gameContext;

   /** The Rocket document representing the shortcut bar GUI */
   Rocket::Core::ElementDocument* m_shortcutBarDocument;

   /** The Rocket element that contains the shortcut bar icons */
   Rocket::Core::Element* m_shortcutContainer;

   public:
      /**
       * Constructor.
       *
       * @param rocketContext The Rocket context to which this console window should attach.
       */
      ShortcutBar(GameContext& gameContext, Rocket::Core::Context& rocketContext);

      /**
       * Destructor.
       */
      ~ShortcutBar();

      void shortcutClicked(Rocket::Core::Event* event);
      void usableDropped(Rocket::Core::Event* event);

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
