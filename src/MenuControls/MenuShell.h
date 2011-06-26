#ifndef MENU_SHELL_H
#define MENU_SHELL_H

#include "Container.h"
#include "MenuAction.h"

namespace gcn
{
   class Icon;
};

namespace edwt
{
   class TabbedArea;
   class ListBox;
   class StringListModel;
   class TabChangeListener;
};

class MenuPane;
class PlayerData;
class Sound;

class MenuShell : public edwt::Container
{
   /** Sound for hovering over an option */
   Sound* reselectSound;

   /** Sound for picking an option */
   Sound* chooseSound;

   /** Background for the menu */
   gcn::Icon* bg;

   /** The list model holding the options for the menu */
   edwt::StringListModel* listOps;

   /** The tabbed container for the main area of the menu */
   edwt::TabbedArea* menuTabs;
      
   /** The main area of the menu */
   edwt::Container* menuArea;

   /**
    * Populate the action list with required options
    */
   void populateOpsList();

   public:
      /** \todo THIS PUBLIC FIELD IS LIKE OMG TEMPORARY. REMOVE IMMEDIATELY! */
      /** The list box for all options in the menu */
      edwt::ListBox* actionsListBox;

      MenuShell(PlayerData& playerData);

      void addPane(MenuPane* menuPane);
      void removePane(MenuPane* menuPane);

      void setTabChangeListener(edwt::TabChangeListener* listener);

      ~MenuShell();

};

#endif
