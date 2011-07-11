#ifndef STATUS_PANE_H_
#define STATUS_PANE_H_

#include "MenuPane.h"

class CharacterModule;
class Character;

/**
 * The menu pane that displays a character's full description and status.
 */
class StatusPane : public MenuPane
{
   /**
    * The character displayed by the status pane.
    */
   Character* character;

   /**
    * This module is a temporary widget to ensure that the status pane displays the correct character.
    * \todo Remove this module after the status pane GUI is completed.
    */
   CharacterModule* module;

   public:
      /**
       * Constructor.
       *
       * @param character The initial character to display in the status pane.
       * @param rect The preferred dimensions for this menu pane.
       */
      StatusPane(Character* character, const gcn::Rectangle& rect);

      /**
       * Change the character displayed by the status pane.
       *
       * @param character The character to display in the status pane.
       */
      void setCharacter(Character* character);

      /**
       * Destructor.
       */
      ~StatusPane();
};

#endif
