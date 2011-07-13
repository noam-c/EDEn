#ifndef ITEMS_PANE_H_
#define ITEMS_PANE_H_

#include "MenuPane.h"
#include <vector>

class PlayerData;
class SaveGameModule;

/**
 * The pane to display when showing the items menu state.
 *
 * @author Noam Chitayat
 */
class DataPane : public MenuPane
{
   /** The player data with the inventory to display. */
   PlayerData& playerData;

   /** The list of files that can be saved or loaded. */
   std::vector<PlayerData*> saveGames;

   /** Save game GUI elements. */
   std::vector<SaveGameModule*> saveGameModules;

   public:
      /**
       * Constructor.
       *
       * @param playerData The player data to display in the pane.
       * @param rect The preferred dimensions of this pane.
       */
      DataPane(PlayerData& playerData, const gcn::Rectangle& rect);

      /**
       * Destructor.
       */
      ~DataPane();
};

#endif
