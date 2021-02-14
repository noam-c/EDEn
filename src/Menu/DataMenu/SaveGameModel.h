/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef SAVE_GAME_MODEL_H
#define SAVE_GAME_MODEL_H

#include <memory>
#include <string>
#include <utility>
#include <vector>

class Metadata;
class PlayerData;
class PlayerDataSummary;
struct SaveLocation;
class SaveMenu;

/**
 * Provides the title screen functionality, including the game logo, backdrop,
 * and options for the user.
 * This state is the first that the user must interact with.
 *
 * @author Noam Chitayat
 */
class SaveGameModel final
{
   SaveMenu& m_saveMenu;

   Metadata& m_metadata;

   /** The list of savegame files and their respective data. */
   std::vector<std::pair<std::string, std::unique_ptr<PlayerDataSummary>>> m_saveGames;

   /**
    * Refreshes the list of save games.
    */
   void refreshSaveGames();

   /**
    * Initializes the data menu pane and populates the sidebar.
    */
   void initialize();

   public:
      /**
       * Constructor.
       */
      SaveGameModel(SaveMenu& saveMenu, Metadata& metadata);

      /**
       * Saves the given player data to the given save slot.
       *
       * @param slotIndex The slot at which the game will be saved.
       */
      void saveToSlot(const PlayerData& playerData, const SaveLocation& saveLocation, size_t index);

      /**
       * Loads player data from the given save slot.
       *
       * @param slotIndex The slot at which the game will be saved.
       */
      std::tuple<std::shared_ptr<PlayerData>, SaveLocation> loadSaveGame(size_t index) const;

      /**
       * @return a read-only reference to a save game.
       */
      const std::unique_ptr<PlayerDataSummary>& getSaveGame(size_t index) const;

      /**
       * @return the number of available saved games.
       */
      size_t getNumSaveGames() const;
};

#endif
