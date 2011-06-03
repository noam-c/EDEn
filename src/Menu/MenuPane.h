#ifndef MENU_PANE_H_
#define MENU_PANE_H_

#include "Container.h"

class MenuPane : public edwt::Container
{
   MenuPane();

   protected:
      MenuPane(const gcn::Rectangle& dimension);
      virtual void draw(gcn::Graphics* graphics);
};

#endif