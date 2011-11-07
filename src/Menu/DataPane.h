#ifndef DATA_PANE_H_
#define DATA_PANE_H_

#include "MenuPane.h"
#include <map>
#include <vector>

namespace edwt
{
   class ModuleSelectListener;
};

class PlayerData;
class SaveGameModule;

/**
 * The pane to display when showing the items menu state.
 *
 * @author Noam Chitayat
 */
class DataPane : public MenuPane, public gcn::ActionListener
{
   /** The module selection listener */
   edwt::ModuleSelectListener* moduleSelectListener;
   
   /** Save game GUI elements. */
   std::vector<SaveGameModule*> saveGameModules;
   
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

      void action(const gcn::ActionEvent& event);
   
      /**
       * Set the save games to display.
       *
       * @param saveGames The savegames to set and display in the pane.
       */
      void setSaveGames(std::vector<PlayerData*> saveGames);

      void setModuleSelectListener(edwt::ModuleSelectListener* listener);
   
      /**
       * Destructor.
       */
      ~DataPane();
};

#endif
