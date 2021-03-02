/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef SHORTCUT_BAR_H
#define SHORTCUT_BAR_H

#include "EdenRocketBindings.h"

enum class GameStateType;

namespace Rml
{
   namespace Core
   {
      class Context;
      class ElementDocument;
   };
};

class Metadata;
class PlayerData;
class ScriptEngine;

class ShortcutBar final
{
   /** The event binding collection for the debug window */
   EdenRocketBindings m_bindings;

   /** The Rocket context that holds this console window */
   Rml::Core::Context& m_rocketContext;

   /** The item and skill metadata */
   Metadata& m_metadata;

   /** The player data providing the shortcuts. */
   PlayerData& m_playerData;

   /** The type of game state driving the shortcuts. */
   const GameStateType m_stateType;

   /** The Rocket document representing the shortcut bar GUI */
   Rml::Core::ElementDocument* m_shortcutBarDocument;

   /** The Rocket element that contains the shortcut bar icons */
   Rml::Core::Element* m_shortcutContainer;

   bool invokeShortcut(int shortcutIndex);

   public:
      /**
       * Constructor.
       *
       * @param rocketContext The Rocket context to which this console window should attach.
       */
      ShortcutBar(PlayerData& playerData, Metadata& metadata, GameStateType stateType, Rml::Core::Context& rocketContext);

      /**
       * Destructor.
       */
      ~ShortcutBar();

      void shortcutClicked(Rml::Core::Event& event);
      void usableDropped(Rml::Core::Event& event);

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
