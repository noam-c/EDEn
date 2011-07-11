#ifndef EQUIP_PANE_H_
#define EQUIP_PANE_H_

#include "MenuPane.h"

namespace edwt
{
   class Container;
   class Label;
};

class Character;
class PlayerData;

/**
 * The pane containing the GUI elements for the character equip menu.
 *
 * @author Noam Chitayat
 */
class EquipPane : public MenuPane
{
   /** The character to display equipment information for. */
   Character* character;

   /** The label for the head slot. */
   edwt::Label* headLabel;

   /** The label containing the head slot equipment (if it exists). */
   edwt::Label* headEquipLabel;
 
   public:
      /**
       * Constructor.
       *
       * @param character The initial character to display in the equipment pane.
       * @param rect The preferred dimensions for this menu pane.
       */
      EquipPane(Character* character, const gcn::Rectangle& rect);

      /**
       * Change the character displayed by the equipment panel.
       *
       * @param character The character to display in the equipment pane.
       */
      void setCharacter(Character* character);

      /**
       * Destructor.
       */
      ~EquipPane();
};

#endif
