#ifndef MENU_STATE_H_
#define MENU_STATE_H_

#include "GameState.h"
#include "TabChangeListener.h"

namespace gcn
{
   class Rectangle;
};

namespace edwt
{
   class Container;
   class TabbedArea;
};

class MenuShell;
class MenuPane;

class MenuState : public GameState, public edwt::TabChangeListener
{
   protected:
      MenuShell& menuShell;
      MenuPane* menuPane;

      MenuState(MenuShell& menuShell);
      void setMenuPane(MenuPane* pane);
      void activate();
      bool step();
      void pollInputEvent(bool& finishState);
      void draw();
      virtual void tabChanged(const std::string& tabName);
      virtual ~MenuState() = 0;
};

#endif