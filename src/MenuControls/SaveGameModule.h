#ifndef CHARACTER_MODULE_H_
#define CHARACTER_MODULE_H_

#include "guichan.hpp"

namespace edwt
{
   class Container;
   class Icon;
   class Label;
};

class PlayerData;
class SaveGameSelectListener;

/**
 * A GUI element used to display summarized character information in a small area of a menu.
 *
 * @author Noam Chitayat
 */
class SaveGameModule : public gcn::contrib::AdjustingContainer, public gcn::MouseListener
{
   /** The player save data to show. */
   PlayerData& playerData;
   
   /** Path to the save game file. */
   const std::string path;
   
   /** Save game selection listener. */
   SaveGameSelectListener* saveGameSelectListener;

   /** The icons holding the portraits of the characters. */
   std::vector<edwt::Icon*> characterPortraits;

   public:
      /**
       * Constructor for creating an empty character module.
       *
       * @param playerData The player data contained in the save game.
       * @param path The path to the save game file.
       */
      SaveGameModule(PlayerData& playerData, const std::string& path);

      /**
       * Captures mouse-click events and fires a character selection listener if needed.
       *
       * @param mouseEvent The mouse input event.
       */
      void mouseClicked(gcn::MouseEvent& mouseEvent);
      
      /**
       * Sets the listener for save game selection events
       * (when a save game in the data pane is clicked).
       * If a listener is already set, it will be disconnected when the new one is set.
       *
       * @param listener The new listener to set.
       */
      void setSaveGameSelectListener(SaveGameSelectListener* listener);

      /**
       * Destructor.
       */
      ~SaveGameModule();
};

#endif
