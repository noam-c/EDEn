/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef DATA_VIEW_MODEL_H
#define DATA_VIEW_MODEL_H

#include <Rocket/Controls/DataSource.h>
#include "ImageFormatter.h"

class PlayerData;

class DataViewModel : public Rocket::Controls::DataSource
{
   PlayerData& playerData;
   const ImageFormatter imageFormatter;
   std::vector<std::pair<std::string, PlayerData*> > saveGames;

   void clearSaveGames();

   public:
      DataViewModel(PlayerData& playerData);
      ~DataViewModel();

      void refreshSaveGames();
      void saveToSlot(int slotIndex);
      void GetRow(Rocket::Core::StringList& row, const Rocket::Core::String& table, int row_index, const Rocket::Core::StringList& columns);
      int GetNumRows(const Rocket::Core::String& table);
};

#endif
