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
   EdenRocketBindings m_bindings;
   
   Character* m_selectedCharacter;

   /** The view model that exposes the inventory to the GUI */
   SkillViewModel m_skillViewModel;

   /**
    * Initializes the skill menu pane and populates the sidebar.
    */
   void initialize();
   
   void skillClicked(Rocket::Core::Event* event);
   void dragStarted(Rocket::Core::Event* event);
   
   void useSkill(int rowIndex);

   public:
      /**
       * Constructor. Initializes the menu GUI.
       *
       * @param gameContext The context containing the current player data and execution stack.
       */
      SkillMenu(GameContext& gameContext);

      /**
       * Constructor. Initializes the menu GUI.
       *
       * @param gameContext The context containing the current player data and execution stack.
       * @param menuShell The shell for the menu.
       */
      SkillMenu(GameContext& gameContext, std::shared_ptr<MenuShell> menuShell);

      /**
       * Destructor.
       */
      ~SkillMenu();

      Character* getSelectedCharacter() const;
      void setCharacter(int characterIndex);
};

#endif
