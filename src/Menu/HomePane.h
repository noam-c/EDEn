/**
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#ifndef HOME_PANE_H
#define HOME_PANE_H

#include "MenuPane.h"

namespace edwt
{
   class ModuleSelectListener;
};

class PlayerData;
class CharacterModule;

/**
 * The GUI elements of the home pane, which is the first menu pane that is shown.
 *
 * @author Noam Chitayat
 */
class HomePane : public MenuPane, public gcn::ActionListener
{
   /** The player data to summarize in this pane. */
   PlayerData& playerData;

   /** The modules used to summarize attributes for all the characters in the party. */
   CharacterModule* characterModules;
   
   /** The listener for character selection events from this pane. */
   edwt::ModuleSelectListener* moduleSelectListener;

   public:
      /**
       * Constructor.
       *
       * @param playerData The player data to display in the pane.
       * @param rect The preferred dimensions of this pane.
       */
      HomePane(PlayerData& playerData, const gcn::Rectangle& rect);

      /**
       * Sets the listener for character selection events
       * (when a character in the home pane is clicked).
       * If a listener is already set, it will be disconnected when the new one is set.
       *
       * @param listener The new listener to set.
       */
      void setModuleSelectListener(edwt::ModuleSelectListener* listener);

      /**
       * Refreshes the character information displayed in the pane.
       */
      void refresh();

      /**
       * Fires when a character is selected in the pane.
       *
       * @param event The action event from the selected module.
       */
      void action(const gcn::ActionEvent& event);
   
      /**
       * Destructor.
       */
      ~HomePane();
};

#endif