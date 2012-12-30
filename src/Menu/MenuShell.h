/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef MENU_SHELL_H
#define MENU_SHELL_H

#include <vector>
#include <string>
#include "EdenRocketBindings.h"
#include "Scheduler.h"

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
   Rocket::Core::Context* rocketContext;

   /** The RML document holding the shell GUI components. */
   Rocket::Core::ElementDocument* shellDocument;

   /**
    * The element containing the sidebar list for
    * the active menu's related actions.
    */
   Rocket::Core::Element* sidebarElement;

   /** The event binding collection for the sidebar */
   EdenRocketBindings<MenuShell> bindings;

   /** The currently active menu state */
   MenuState* currentState;

   /** The menu's script scheduler */
   Scheduler* scheduler;

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
   void sidebarClicked(Rocket::Core::Event* event);

   public:
      /**
       * Constructor.
       *
       * @param context The menu context that will contain the menu shell GUI.
       */
      MenuShell(Rocket::Core::Context* context);

      /**
       * Destructor.
       */
      ~MenuShell();

      /**
       * @return The Rocket context managing the shell GUI.
       */
      Rocket::Core::Context* getRocketContext() const;

      /**
       * @return The scheduler used to manage the menu coroutines.
       */
      Scheduler* getScheduler() const;

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
