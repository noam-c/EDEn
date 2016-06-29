/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef ITEM_VIEW_MODEL_H
#define ITEM_VIEW_MODEL_H

#include <Rocket/Controls/DataSource.h>
#include "ImageFormatter.h"
#include "UsableId.h"

class Metadata;
class PlayerData;

/**
 * A view model that allows a Rocket GUI to bind to the player's inventory.
 *
 * @author Noam Chitayat
 */
class ItemViewModel : public Rocket::Controls::DataSource
{
   const static Rocket::Core::String UnknownItemIconPath;

   const Metadata& m_metadata;
   PlayerData& m_playerData;

   /** The formatter to use when exposing images to the GUI. */
   const ImageFormatter m_imageFormatter;

   public:
      /**
       * Constructor.
       */
      ItemViewModel(const Metadata& metadata, PlayerData& playerData);

      UsableId getItemId(int rowIndex) const;

      /**
       * Populates <code>row</code> with the specified columns of a row of data specified by <code>row_index</code>.
       *
       * @param row The list to populate with values from the desired row.
       * @param table The name of the table to query.
       * @param row_index The index of the desired row.
       * @param columns The list of desired columns within the row.
       */
      void GetRow(Rocket::Core::StringList& row, const Rocket::Core::String& table, int row_index, const Rocket::Core::StringList& columns) override;

      /**
       * @param table The table to query for row count.
       *
       * @return The number of rows in the specified table.
       */
      int GetNumRows(const Rocket::Core::String& table) override;

      void refresh(int rowIndex);
};

#endif
