/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef SHORTCUT_H
#define SHORTCUT_H

#include "UsableId.h"
#include <string>

namespace Json
{
   class Value;
};

struct Shortcut final
{
   static const char* ID_ATTRIBUTE;
   static const char* TYPE_ATTRIBUTE;
   static const char* CHARACTER_ID_ATTRIBUTE;

   enum class UsableType
   {
      EMPTY = 0,
      ITEM = 1,
      SKILL = 2,
   };

   UsableType usableType = UsableType::EMPTY;
   UsableId usableId = 0;
   std::string characterId;

   Shortcut(UsableId itemId);
   Shortcut(UsableId skillId, const std::string& characterId);
   void load(Json::Value& shortcutJson);
   Json::Value serialize() const;

   static Shortcut getEmptyShortcut();

   private:
      Shortcut();
};

#endif

