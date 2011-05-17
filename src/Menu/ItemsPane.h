#ifndef ITEMS_PANE_H_
#define ITEMS_PANE_H_

#include "MenuPane.h"

class PlayerData;

namespace edwt
{
   class ListBox;
   class StringListModel;
};

class ItemsPane : public MenuPane
{
   PlayerData& playerData;
   edwt::ListBox* listBox;
   edwt::StringListModel* itemsList;

   public:
      ItemsPane(PlayerData& playerData, const gcn::Rectangle& rect);
      void refresh();
      ~ItemsPane();
};

#endif