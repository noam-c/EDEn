#ifndef MENU_PANE_H_
#define MENU_PANE_H_

#include "guichan.hpp"

class MenuPane : public gcn::Container
{
   MenuPane();

   protected:
      MenuPane(const gcn::Rectangle& dimension);
};

#endif