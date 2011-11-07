#ifndef MENU_PANE_H_
#define MENU_PANE_H_

#include "Container.h"

/**
 * A menu pane represents the panel shown by a menu state.
 * When creating a menu state, create a subclass of menu pane to contain the menu state's UI,
 * and construct an instance in the Menu State.
 *
 * @author Noam Chitayat
 */
class MenuPane : public edwt::Container
{
   /**
    * Overrides the default constructor, since the protected constructor below should be used instead.
    */
   MenuPane();
   
   protected:
      /**
       * Constructor.
       *
       * @param dimension The preferred dimensions of this menu pane.
       */
      MenuPane(const gcn::Rectangle& dimension);
   
      /**
       * Updates the menu pane.
       */
      virtual void logic();

      /**
       * Draws the menu pane.
       *
       * @param graphics The graphics instance to draw with.
       */
      virtual void draw(gcn::Graphics* graphics);

   public:
      /**
       * Destructor.
       */
      virtual ~MenuPane() = 0;
};

#endif