/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "Shortcut.h"
#include "json.h"

const char* Shortcut::ID_ATTRIBUTE = "id";
const char* Shortcut::TYPE_ATTRIBUTE = "type";
const char* Shortcut::CHARACTER_ID_ATTRIBUTE = "character";

Shortcut::Shortcut() :
   usableType(EMPTY),
   usableId(0)
{
}

Shortcut::Shortcut(UsableId itemId) :
   usableType(ITEM),
   usableId(itemId)
{
}

Shortcut::Shortcut(UsableId skillId, const std::string& characterId) :
   usableType(SKILL),
   usableId(skillId),
   characterId(characterId)
{
}

Shortcut Shortcut::getEmptyShortcut()
{
   return Shortcut();
}

void Shortcut::load(Json::Value& shortcutJson)
{
   usableType = EMPTY;
   usableId = 0;
   characterId = "";

   if(!shortcutJson.isNull())
   {
      UsableId loadedUsableId = shortcutJson[Shortcut::ID_ATTRIBUTE].asUInt();
      int loadedUsableType = shortcutJson[Shortcut::TYPE_ATTRIBUTE].asInt();
      std::string loadedCharacterId = shortcutJson[Shortcut::CHARACTER_ID_ATTRIBUTE].asString();

      if(loadedUsableType == static_cast<int>(ITEM) &&
         loadedUsableId > 0)
      {
         usableType = ITEM;
         usableId = loadedUsableId;
      }
      else if(loadedUsableType == static_cast<int>(SKILL) &&
              loadedUsableId > 0 &&
              !loadedCharacterId.empty())
      {
         usableType = SKILL;
         usableId = loadedUsableId;
         characterId = loadedCharacterId;
      }
   }
}

Json::Value Shortcut::serialize() const
{
   Json::Value serializedShortcut(Json::nullValue);

   if(usableType == ITEM && usableId > 0)
   {
      serializedShortcut = Json::Value(Json::objectValue);
      serializedShortcut[Shortcut::ID_ATTRIBUTE] = usableId;
      serializedShortcut[Shortcut::TYPE_ATTRIBUTE] = usableType;
   }
   else if(usableType == SKILL && usableId > 0 && !characterId.empty())
   {
      serializedShortcut = Json::Value(Json::objectValue);
      serializedShortcut[Shortcut::ID_ATTRIBUTE] = usableId;
      serializedShortcut[Shortcut::TYPE_ATTRIBUTE] = static_cast<int>(usableType);
      serializedShortcut[Shortcut::CHARACTER_ID_ATTRIBUTE] = characterId;
   }

   return serializedShortcut;
}

