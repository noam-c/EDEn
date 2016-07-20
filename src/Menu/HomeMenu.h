/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef HOME_MENU_H
#define HOME_MENU_H

#include <stack>
#include <map>

#include "MenuState.h"
#include "EdenRocketBindings.h"
#include "HomeViewModel.h"

namespace Rocket
{
   namespace Core
   {
      class Context;
      class Element;
      class Event;
   };
};

class Sound;

/**
 * The home menu is the first menu state of the menu.
 * It serves to summarize important information for the player and allows the player to select
 * a character to perform further actions on.
 *
 * @author Noam Chitayat
 */
class HomeMenu final : public MenuState
{
   /** The event binding collection for this GUI */
   EdenRocketBindings m_bindings;

   /** The view model */
   HomeViewModel m_homeViewModel;

   int m_selectedDestinationMenu = -1;

   void pushCharacterIndependentMenu(int optionIndex);
   void pushCharacterDependentMenu(int optionIndex, int characterIndex);

   /**
    * Initializes the home menu pane and populates the sidebar.
    */
   void initialize();

   /**
    * @param event The click event that occurred in the party list.
    */
   void characterClicked(Rocket::Core::Event& event);

   /**
    * Signals to the view model that a character was selected in the menu.
    *
    * @param slotIndex The character slot that was selected.
    * @param menuShell The menu shell shared between the menus.
    */
   void selectCharacter(int slotIndex);

   public:
      /**
       * Constructor. Initializes the menu GUI.
       *
       * @param gameContext The context containing the current player data and execution stack.
       */
      HomeMenu(GameContext& gameContext, PlayerData& playerData);

      /**
       * Constructor. Initializes the menu GUI.
       *
       * @param gameContext The context containing the current player data and execution stack.
       * @param menuShell The shell for the menu
       */
      HomeMenu(GameContext& gameContext, PlayerData& playerData, std::shared_ptr<MenuShell> menuShell);

      /**
       * Destructor.
       */
      ~HomeMenu() override;

      /**
       * Handles sidebar option click events by navigating to the
       * appropriate menu.
       *
       * @param optionIndex The index of the sidebar option that was clicked.
       */
      void sidebarClicked(int optionIndex) override;
};

#endif
