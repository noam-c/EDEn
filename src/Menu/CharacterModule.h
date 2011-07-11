#ifndef CHARACTER_MODULE_H_
#define CHARACTER_MODULE_H_

#include "guichan.hpp"

namespace edwt
{
   class Container;
   class Icon;
   class Label;
};

class Character;
class CharacterSelectListener;

/**
 * A GUI element used to display summarized character information in a small area of a menu.
 *
 * @author Noam Chitayat
 */
class CharacterModule : public gcn::contrib::AdjustingContainer, public gcn::MouseListener
{
   /** A resizing container that displays some important character attributes. */
   gcn::contrib::AdjustingContainer* characterStats;

   /** Character selection listener. */
   CharacterSelectListener* charSelectListener;

   /** The name of the character displayed in this module. */
   std::string characterName;

   /** The label displaying the character name. */
   edwt::Label* characterNameLabel;

   /** The label displaying the HP of the character. */
   edwt::Label* characterHPLabel;

   /** The label displaying the SP of the character. */
   edwt::Label* characterSPLabel;

   /** The icon holding the portrait of the character. */
   edwt::Icon* characterPortrait;

   public:
      /**
       * Constructor for creating an empty character module.
       */
      CharacterModule();

      /**
       * Captures mouse-click events and fires a character selection listener if needed.
       *
       * @param mouseEvent The mouse input event.
       */
      void mouseClicked(gcn::MouseEvent& mouseEvent);

      /**
       * Sets the listener for character selection events
       * (when a character in the home pane is clicked).
       * If a listener is already set, it will be disconnected when the new one is set.
       *
       * @param listener The new listener to set.
       */
      void setCharacterSelectListener(CharacterSelectListener* listener);

      /**
       * Sets the character for the module to summarize.
       *
       * @param character The character to display and summarize in the module.
       */
      void setCharacter(Character* character);

      /**
       * Destructor.
       */
      ~CharacterModule();
};

#endif