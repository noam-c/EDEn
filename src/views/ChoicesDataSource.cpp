/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "ChoicesDataSource.h"
#include "DialogueController.h"

#include <algorithm>
#include <string>
#include <vector>

#include "DebugUtils.h"
#define DEBUG_FLAG DEBUG_DIA_CONTR

ChoicesDataSource::ChoicesDataSource(DialogueController& choiceProvider) :
   Rocket::Controls::DataSource("choicesDataSource"),
   m_choiceProvider(choiceProvider)
{
}

void ChoicesDataSource::GetRow(Rocket::Core::StringList& row,
      const Rocket::Core::String& table, int row_index,
      const Rocket::Core::StringList& columns)
{
   if (table == "choices" && m_choiceProvider.hasDialogue())
   {
      auto& choices = m_choiceProvider.getCurrentChoices();
      for (int i = 0; i < columns.size(); ++i)
      {
         if (columns[i] == "text")
         {
            row.emplace_back(choices[row_index].c_str());
         }
      }
   }
}

int ChoicesDataSource::GetNumRows(const Rocket::Core::String& table)
{
   if (table == "choices")
   {
      return m_choiceProvider.hasDialogue() ?
         m_choiceProvider.getCurrentChoices().size() : 0;
   }

   return 0;
}

void ChoicesDataSource::refresh()
{
   NotifyRowChange("choices");
}
