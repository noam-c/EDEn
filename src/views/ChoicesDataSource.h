/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef CHOICES_VIEW_MODEL_H
#define CHOICES_VIEW_MODEL_H

#include <Rocket/Controls/DataSource.h>

class ChoiceProvider;

/**
 * A view model that allows a Rocket GUI to bind to a set of dialogue choices.
 *
 * @author Noam Chitayat
 */
class ChoicesDataSource final : public Rocket::Controls::DataSource
{
   ChoiceProvider& m_choiceProvider;

   public:
      /**
       * Constructor.
       */
      ChoicesDataSource(ChoiceProvider& choiceProvider);

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

      void refresh();
};

#endif
