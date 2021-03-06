/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "GameState.h"
#include "MenuShellOption.h"

#include <memory>
#include <vector>

namespace Rocket
{
   namespace Core
   {
      class Context;
      class ElementDocument;
   };
};

class MenuShell;
class ShortcutBar;
class GameContext;

/**
 * The in-game menu goes through many different states based on choices that the player makes
 * and the panels that appear. The states need to be tracked in a FIFO order so that cancelling
 * one menu state returns the user to the previous one.
 *
 * In order to accomplish this, the in-game menu is split into multiple menu-specific GameStates called MenuStates.
 * All the <code>MenuState</code> objects share the same GUI elements, except for the menu pane on display.
 * As a result, these shared elements are combined into a <code>MenuShell</code>, which is passed around
 * between all the menu states to facilitate reuse of existing GUI objects.
 *
 * To create a new menu action, simply create a subclass of <code>MenuState</code>, and then create and load your desired RML in the subclass
 * constructor.
 * You can add sidebar options by populating the <code>sidebarOptions</code> vector on construction.
 *
 * Ensure that the menu shell (and other common data) is passed up to the MenuState through its constructor.
 *
 * @author Noam Chitayat
 */
class MenuState : public GameState
{
   /**
    * Wait for and handle the input event.
    *
    * @param finishState Returned as true if the input event quit out of the main menu.
    */
   void waitForInputEvent(bool& finishState);

   protected:
      /** The player data backing the menu state. */
      PlayerData& m_playerData;

      /** The shell that holds the common menu controls */
      std::shared_ptr<MenuShell> m_menuShell;

      ShortcutBar* m_shortcutBar = nullptr;

      /** The list of sidebar options associated with this menu state. */
      std::vector<MenuShellOption> m_sidebarOptions;

      /** The menu pane RML document */
      Rocket::Core::ElementDocument* m_paneDocument;

      /**
       * Sets this menu state to be the current one.
       */
      virtual void activate() override;

      /**
       * Removes this menu state from active use.
       */
      virtual void deactivate() override;

      /**
       * @return The scheduler used to manage the menu's coroutines
       */
      Scheduler* getScheduler() override;

      /**
       * Draws the menu.
       */
      void draw() override;

      /**
       * Perform logic for the menu screen.
       *
       * @return true iff the user is not finished with the menu.
       */
      bool step(long timePassed) override;

   public:
      /**
       * Constructor.
       *
       * @param gameContext The context containing the current player data and execution stack.
       * @param stateName The unique name of the state.
       */
      MenuState(GameContext& gameContext, PlayerData& playerData, const std::string& stateName);

      /**
       * Constructor.
       *
       * @param gameContext The context containing the current player data and execution stack.
       * @param stateName The unique name of the state.
       * @param shell The shell for the menu.
       */
      MenuState(GameContext& gameContext, PlayerData& playerData, const std::string& stateName, std::shared_ptr<MenuShell> menuShell);

      /**
       * Destructor.
       */
      virtual ~MenuState() override = 0;

      /**
       * @return A list of options that are displayed on the menu sidebar.
       */
      std::vector<MenuShellOption> getSidebarOptions();

      /**
       * Called to notify a <code>MenuState</code> that a sidebar option was clicked.
       *
       * @param optionIndex The index of the selected option within the <code>sidebarOptions</code> list.
       */
      virtual void sidebarClicked(int optionIndex);

      /**
       * Toggles the menu sidebar on/off.
       *
       * @param enabled true iff the sidebar should be enabled
       */
      void setSidebarEnabled(bool enabled);
};

#endif
