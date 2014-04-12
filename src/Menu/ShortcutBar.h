/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef SHORTCUT_BAR_H
#define SHORTCUT_BAR_H

#include "EdenRocketBindings.h"

/**
 * \todo Change this include to forward declarations when enums
 * can be forward declared in the product.
 */
#include "GameStateType.h"

namespace Rocket
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

class ShortcutBar
{
   /** The event binding collection for the debug window */
   EdenRocketBindings m_bindings;

   /** The Rocket context that holds this console window */
   Rocket::Core::Context& m_rocketContext;

   /** The item and skill metadata */
   const Metadata& m_metadata;
   
   /** The player data providing the shortcuts. */
   PlayerData& m_playerData;
   
   /** The script engine to use to run a shortcut's script. */
   ScriptEngine& m_scriptEngine;
   
   /** The type of game state driving the shortcuts. */
   const GameStateType m_stateType;

   /** The Rocket document representing the shortcut bar GUI */
   Rocket::Core::ElementDocument* m_shortcutBarDocument;

   /** The Rocket element that contains the shortcut bar icons */
   Rocket::Core::Element* m_shortcutContainer;

   bool invokeShortcut(int shortcutIndex);

   public:
      /**
       * Constructor.
       *
       * @param rocketContext The Rocket context to which this console window should attach.
       */
      ShortcutBar(PlayerData& playerData, ScriptEngine& scriptEngine, const Metadata& metadata, GameStateType stateType, Rocket::Core::Context& rocketContext);

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
