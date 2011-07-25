#ifndef DATA_PANE_H_
#define DATA_PANE_H_

#include "MenuPane.h"
#include <map>
#include <vector>

class PlayerData;
class SaveGameModule;
class SaveGameSelectListener;

/**
 * The pane to display when showing the items menu state.
 *
 * @author Noam Chitayat
 */
class DataPane : public MenuPane
{
   /** Save game GUI elements. */
   std::vector<SaveGameModule*> saveGameModules;
   
   /** The listener for the save game selection events. */
   SaveGameSelectListener* saveGameSelectListener;
   
   /**
    * Clear out old modules to refresh the GUI.
    */
   void clearModules();
   
   public:
      /**
       * Constructor.
       *
       * @param playerData The player data to display in the pane.
       * @param rect The preferred dimensions of this pane.
       */
      DataPane(const gcn::Rectangle& rect);

      /**
       * Set the save games to display.
       *
       * @param saveGames The savegames to set and display in the pane.
       */
      void setSaveGames(std::map<std::string, PlayerData*> saveGames);
   
      /**
       * Sets a new listener to catch save game selection events.
       *
       * @param listener The listener that will respond to save data selection.
       */
      void setSaveGameSelectListener(SaveGameSelectListener* listener);
   
      /**
       * Destructor.
       */
      ~DataPane();
};

#endif
