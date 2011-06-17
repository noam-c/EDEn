#ifndef MENU_STATE_H_
#define MENU_STATE_H_

#include "GameState.h"

namespace gcn
{
   class Rectangle;
};

namespace edwt
{
   class Container;
};

class MenuState : public GameState
{
   protected:
      edwt::Container& menuPane;

      MenuState(edwt::Container* top, edwt::Container& menuPane);
      bool step();
      void pollInputEvent(bool& finishState);
      void draw();
      virtual ~MenuState() = 0;
};

#endif