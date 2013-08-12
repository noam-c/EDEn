/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef SKILL_MENU_H
#define SKILL_MENU_H

#include "CharacterDependentMenu.h"
#include "EdenRocketBindings.h"
#include "SkillViewModel.h"
#include "UsableId.h"

namespace Rocket
{
   namespace Core
   {
      class Context;
      class Element;
      class Event;
   };
};

class PlayerData;
class Sound;

/**
 * The home menu is the first menu state of the menu.
 * It serves to summarize important information for the player and allows the player to select
 * a character to perform further actions on.
 *
 * @author Noam Chitayat
 */
class SkillMenu: public CharacterDependentMenu
{
   /** The event binding collection for this GUI */
   EdenRocketBindings<SkillMenu> m_bindings;

   /** The view model that exposes the inventory to the GUI */
   SkillViewModel m_skillViewModel;

   /**
    * Initializes the skill menu pane and populates the sidebar.
    */
   void initialize();

   protected:
      /**
       * Activates the data menu and reveals the data pane.
       */
      void activate();

      /**
       * Deactivates the data menu and hides the data pane.
       */
      void deactivate();

   public:
      /**
       * Constructor. Initializes the menu GUI.
       *
       * @param gameContext The context containing the current player data and execution stack.
       * @param playerData The player data that the menu will display.
       */
      SkillMenu(GameContext& gameContext, PlayerData& playerData);

      /**
       * Constructor. Initializes the menu GUI.
       *
       * @param gameContext The context containing the current player data and execution stack.
       * @param playerData The player data that the menu will display.
       * @param menuShell The shell for the menu.
       */
      SkillMenu(GameContext& gameContext, PlayerData& playerData, MenuShell* menuShell);

      /**
       * Destructor.
       */
      ~SkillMenu();

      void setCharacter(int characterIndex);

      void skillClicked(Rocket::Core::Event* event);
      void dragStarted(Rocket::Core::Event* event);
};

#endif
