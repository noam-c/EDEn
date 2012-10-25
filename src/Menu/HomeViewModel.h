/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef HOME_VIEW_MODEL_H
#define HOME_VIEW_MODEL_H

#include <Rocket/Controls/DataSource.h>
#include "ImageFormatter.h"

class CharacterRoster;

class HomeViewModel : public Rocket::Controls::DataSource
{
   const CharacterRoster& characterRoster;
   const ImageFormatter imageFormatter;

   public:
      HomeViewModel(const CharacterRoster& characterRoster);
      ~HomeViewModel();

      void GetRow(Rocket::Core::StringList& row, const Rocket::Core::String& table, int row_index, const Rocket::Core::StringList& columns);
      int GetNumRows(const Rocket::Core::String& table);
};

#endif