/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#ifndef MENU_SHELL_H
#define MENU_SHELL_H

#include <memory>
#include <vector>
#include <string>
#include "EdenRocketBindings.h"
#include "Scheduler.h"

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
      class Element;
      class Event;
   };
};

class MenuState;
class Metadata;
class PlayerData;
class ScriptEngine;
class ShortcutBar;

/**
 * The menu shell contains the foundations of the in-game menu.
 * All the standard GUI elements, such as the Rocket context, the action sidebar, and the background
 * are created in the shell, which is passed up through any menu-based GameState (MenuState types) to allow for
 * sharing of a single shell among states.
 *
 * @author Noam Chitayat
 */
class MenuShell
{
   /** The Rocket context which holds the shell's GUI components. */
   Rocket::Core::Context* m_rocketContext;

   /** The RML document holding the shell GUI components. */
   Rocket::Core::ElementDocument* m_shellDocument;

   /**
    * The element containing the sidebar list for
    * the active menu's related actions.
    */
   Rocket::Core::Element* m_sidebarElement;

   /**
    * The shortcut bar containing the player's set of shortcuts.
    */
   std::unique_ptr<const ShortcutBar> m_shortcutBar;

   /** The event binding collection for the sidebar */
   EdenRocketBindings m_bindings;

   /** The currently active menu state */
   MenuState* m_currentState;

   /** The menu's script scheduler */
   Scheduler m_scheduler;

   /**
    * Refreshes the menu shell GUI components.
    */
   void refresh();

   /**
    * Event handler for sidebar element clicks.
    * Determines the clicked option index and sends an
    * event to the currently active menu.
    *
    * @param event The click event that occurred in the sidebar.
    */
   void sidebarClicked(Rocket::Core::Event& event);

   public:
      /**
       * Constructor.
       *
       * @param rocketContext The menu context that will contain the menu shell GUI.
       */
      MenuShell(Rocket::Core::Context* rocketContext);

      /**
       * Destructor.
       */
      ~MenuShell();

      void initializeShortcutBar(PlayerData& playerData, Metadata& metadata, GameStateType stateType);

      /**
       * @return The Rocket context managing the shell GUI.
       */
      Rocket::Core::Context* getRocketContext() const;

      /**
       * @return The scheduler used to manage the menu coroutines.
       */
      Scheduler* getScheduler();

      /**
       * @return The RML document managing the shell GUI.
       */
      Rocket::Core::ElementDocument* getShellDocument() const;

      /**
       * Sets a new active menu state.
       *
       * @param newState The menu state to change to.
       */
      void changeMenuState(MenuState* newState);
};

#endif
