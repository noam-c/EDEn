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
   
   /** The icons holding the portraits of the characters. */
   std::vector<edwt::Icon*> characterPortraits;

   public:
      /**
       * Constructor for creating an empty character module.
       */
      SaveGameModule(PlayerData& playerData);

      /**
       * Captures mouse-click events and fires a character selection listener if needed.
       *
       * @param mouseEvent The mouse input event.
       */
      void mouseClicked(gcn::MouseEvent& mouseEvent);

      /**
       * Destructor.
       */
      ~SaveGameModule();
};

#endif
