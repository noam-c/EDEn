/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "GameState.h"
#include "MenuShell.h"
#include "MenuShellOption.h"

#include <vector>

class MenuState : public GameState
{
   /**
    * Wait for and handle the input event.
    *
    * @param finishState Returned as true if the input event quit out of the main menu.
    */
   void waitForInputEvent(bool& finishState);

   protected:
      /** The shell that holds the common menu controls */
      MenuShell menuShell;

      std::vector<MenuShellOption> sidebarOptions;

      /**
       * Sets this menu state to be the current one.
       */
      virtual void activate();

      /**
       * Draws the menu.
       */
      void draw();

      /**
       * Perform logic for the menu screen.
       *
       * @return true iff the user is not finished with the menu.
       */
      bool step();

   public:
      MenuState(ExecutionStack& executionStack);
      MenuState(ExecutionStack& executionStack, MenuShell& menuShell);
      virtual ~MenuState() = 0;

      std::vector<MenuShellOption> getSidebarOptions();
      virtual void sidebarClicked(int optionIndex) = 0;
};

#endif
