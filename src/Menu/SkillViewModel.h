/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef SKILL_VIEW_MODEL_H
#define SKILL_VIEW_MODEL_H

#include <RmlUi/Controls/DataSource.h>
#include "ImageFormatter.h"
#include "UsableId.h"

class Character;
class GameContext;
class Metadata;
class SkillMenu;

/**
 * A view model that allows a Rocket GUI to bind to the player's inventory.
 *
 * @author Noam Chitayat
 */
class SkillViewModel final : public Rml::Controls::DataSource
{
   const static Rml::Core::String UnknownSkillIconPath;

   SkillMenu& m_skillMenu;
   const Metadata& m_metadata;

   /** The formatter to use when exposing images to the GUI. */
   const ImageFormatter m_imageFormatter;

   public:
      /**
       * Constructor.
       */
      SkillViewModel(SkillMenu& skillMenu, const Metadata& metadata);

      void setCharacter(Character* character);

      UsableId getSkillId(int rowIndex) const;

      std::string getCurrentCharacterId() const;

      /**
       * Populates <code>row</code> with the specified columns of a row of data specified by <code>row_index</code>.
       *
       * @param row The list to populate with values from the desired row.
       * @param table The name of the table to query.
       * @param row_index The index of the desired row.
       * @param columns The list of desired columns within the row.
       */
      void GetRow(Rml::Core::StringList& row, const Rml::Core::String& table, int row_index, const Rml::Core::StringList& columns) override;

      /**
       * @param table The table to query for row count.
       *
       * @return The number of rows in the specified table.
       */
      int GetNumRows(const Rml::Core::String& table) override;

      void refresh();
      void refresh(int rowIndex);
};

#endif
