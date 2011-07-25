#ifndef SAVE_GAME_SELECT_LISTENER_H
#define SAVE_GAME_SELECT_LISTENER_H

#include <string>

/**
 * An interface for classes that detect and act on savegame selection events in the in-game menu.
 *
 * @author Noam Chitayat
 */
class SaveGameSelectListener
{
   public:
      /**
       * Fires when a save game is selected from a data display in the in-game menu.
       *
       * @param path The path of the save game that was selected.
       */
      virtual void saveGameSelected(const std::string& path) = 0;
};

#endif
