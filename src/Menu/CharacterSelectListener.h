#ifndef CHARACTER_SELECT_LISTENER_H
#define CHARACTER_SELECT_LISTENER_H

#include <string>

/**
 * An interface for classes that detect and act on character selection events in the in-game menu.
 *
 * @author Noam Chitayat
 */
class CharacterSelectListener
{
   public:
      /**
       * Fires when a character is selected from a character display in the in-game menu.
       *
       * @param characterName The name of the character that was selected.
       */
      virtual void characterSelected(const std::string& characterName) = 0;
};

#endif