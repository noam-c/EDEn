/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#ifndef HOME_VIEW_MODEL_H
#define HOME_VIEW_MODEL_H

#include <Rocket/Controls/DataSource.h>
#include "ImageFormatter.h"

class PlayerData;

/**
 * A view model that allows a Rocket GUI to bind to player data.
 * Specifically, this view model exposes summary data about the player's
 * current party.
 *
 * @author Noam Chitayat
 */
class HomeViewModel : public Rocket::Controls::DataSource
{
   /** The player data backing the menu state */
   PlayerData& m_playerData;

   /** The formatter to use when exposing images to the GUI. */
   const ImageFormatter m_imageFormatter;

   public:
      /**
       * Constructor.
       *
       * @param playerData The player data backing the menu state.
       */
      HomeViewModel(PlayerData& playerData);

      /**
       * Populates <code>row</code> with the specified columns of a row of data specified by <code>row_index</code>.
       *
       * @param row The list to populate with values from the desired row.
       * @param table The name of the table to query.
       * @param row_index The index of the desired row.
       * @param columns The list of desired columns within the row.
       */
      void GetRow(Rocket::Core::StringList& row, const Rocket::Core::String& table, int row_index, const Rocket::Core::StringList& columns);

      /**
       * @param table The table to query for row count.
       *
       * @return The number of rows in the specified table.
       */
      int GetNumRows(const Rocket::Core::String& table);
};

#endif
