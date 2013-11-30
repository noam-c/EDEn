/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef DATA_VIEW_MODEL_H
#define DATA_VIEW_MODEL_H

#include <Rocket/Controls/DataSource.h>
#include "ImageFormatter.h"

class GameContext;
class PlayerDataSummary;

/**
 * A view model that allows a Rocket GUI to bind to the player's saved games.
 * Specifically, this view model exposes summary data about each saved game
 * in the player's savegames directory.
 *
 * @author Noam Chitayat
 */
class DataViewModel : public Rocket::Controls::DataSource
{
   GameContext& m_gameContext;

   /** The formatter to use when exposing images to the GUI. */
   const ImageFormatter m_imageFormatter;

   /** The list of savegame files and their respective data. */
   std::vector<std::pair<std::string, PlayerDataSummary*> > m_saveGames;

   /**
    * Clears the list of savegames.
    * Used during destruction or refresh.
    */
   void clearSaveGameList();

   public:
      /**
       * Constructor.
       */
      DataViewModel(GameContext& gameContext);

      /**
       * Destructor.
       */
      ~DataViewModel();

      /**
       * Refreshes the list of save games.
       */
      void refreshSaveGames();

      /**
       * Saves the current player data to the given save slot.
       *
       * @param slotIndex The slot at which the game will be saved.
       */
      void saveToSlot(int slotIndex);

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
