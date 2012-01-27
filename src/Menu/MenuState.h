/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "GameState.h"
#include "TabChangeListener.h"
#include "MenuAction.h"

namespace gcn
{
   class Rectangle;
};

namespace edwt
{
   class Container;
   class TabbedArea;
};

class MenuShell;
class MenuPane;

/**
 * The in-game menu goes through many different states based on choices that the player makes
 * and the panels that appear. The states need to be tracked in a FIFO order so that cancelling
 * one menu state returns the user to the previous one.
 *
 * In order to accomplish this, the in-game menu is split into multiple menu-specific GameStates called MenuStates.
 * All the MenuStates share the same GUI elements, except for the menu pane on display. As a result, these shared elements
 * are combined into a MenuShell, which is passed around between all the menu states to facilitate reuse of existing GUI objects.
 *
 * To create a new menu action, simply create a subclass of MenuState, and then create and set a menu pane in the subclass
 * constructor. Ensure that the menu shell (and other common data) is passed up to the MenuState through its constructor.
 * Override the virtual methods below to handle user input and add any special logic or drawing.
 *
 * @author Noam Chitayat
 */
class MenuState : public GameState, public edwt::TabChangeListener
{
   /**
    * Polls for and acts on generic menu events.
    *
    * @param finishState Set to true if an event ordered the completion of this state (such as "Cancel")
    */
   void pollInputEvent(bool& finishState);

   protected:
      /** The menu shell that encases this state's GUI. */
      MenuShell& menuShell;

      /** The state's GUI panel. */
      MenuPane* menuPane;

      /**
       * Constructor.
       *
       * @param executionStack The execution stack that the state belongs to.
       * @param menuShell The menu shell to place the menu pane into.
       */
      MenuState(ExecutionStack& executionStack, MenuShell& menuShell);
   
      /**
       * Set the menu pane that will appear within the menu shell for this state.
       * NOTE: The MenuState will delete the pane when cleaned up, so please DO NOT delete the menu pane from the subclass.
       *
       * @param pane The menu pane to display.
       */
      void setMenuPane(MenuPane* pane);

      /**
       * Processes for events common to all menu states, such as "cancel" actions.
       */
      virtual bool step();
   
      /**
       * Common menu drawing code should go here.
       * Currently, the GUI takes care of all of that, so this method will remain empty for now.
       */
      virtual void draw();

      /**
       * Destructor.
       */
      virtual ~MenuState() = 0;

   public:      
      /**
       * When this state is active, activate makes sure this state's menu pane
       * is visible and listening for tab change events.
       */
      virtual void activate();

      /**
       * Override this method to respond to menu tab events.
       *
       * @param tabName The name of the tab that was selected by the user.
       */
      virtual void tabChanged(const std::string& tabName);
      
      /**
       * Respond to a new action being chosen from the menu list.
       *
       * @param The selected menu action.
       */
      virtual void showPanel(MenuAction panelToShow);
};

#endif
