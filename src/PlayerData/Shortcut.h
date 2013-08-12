/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef SHORTCUT_H
#define SHORTCUT_H

#ifndef NULL
   #define NULL 0
#endif

#include "UsableId.h"
#include <string>

namespace Json
{
   class Value;
};

struct Shortcut
{
   static const char* ID_ATTRIBUTE;
   static const char* TYPE_ATTRIBUTE;
   static const char* CHARACTER_ID_ATTRIBUTE;

   enum UsableType
   {
      EMPTY = 0,
      ITEM = 1,
      SKILL = 2,
   };

   UsableType usableType;
   UsableId usableId;
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

