/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include "Shortcut.h"
#include "json.h"

const char* Shortcut::ID_ATTRIBUTE = "id";
const char* Shortcut::TYPE_ATTRIBUTE = "type";
const char* Shortcut::CHARACTER_ID_ATTRIBUTE = "character";

Shortcut::Shortcut() :
   usableType(UsableType::EMPTY),
   usableId(0)
{
}

Shortcut::Shortcut(UsableId itemId) :
   usableType(UsableType::ITEM),
   usableId(itemId)
{
}

Shortcut::Shortcut(UsableId skillId, const std::string& characterId) :
   usableType(UsableType::SKILL),
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
   usableType = UsableType::EMPTY;
   usableId = 0;
   characterId = "";

   if(!shortcutJson.isNull())
   {
      UsableId loadedUsableId = shortcutJson[Shortcut::ID_ATTRIBUTE].asUInt();
      int loadedUsableType = shortcutJson[Shortcut::TYPE_ATTRIBUTE].asInt();
      std::string loadedCharacterId = shortcutJson[Shortcut::CHARACTER_ID_ATTRIBUTE].asString();

      if(loadedUsableType == static_cast<int>(UsableType::ITEM) &&
         loadedUsableId > 0)
      {
         usableType = UsableType::ITEM;
         usableId = loadedUsableId;
      }
      else if(loadedUsableType == static_cast<int>(UsableType::SKILL) &&
              loadedUsableId > 0 &&
              !loadedCharacterId.empty())
      {
         usableType = UsableType::SKILL;
         usableId = loadedUsableId;
         characterId = loadedCharacterId;
      }
   }
}

Json::Value Shortcut::serialize() const
{
   Json::Value serializedShortcut(Json::nullValue);

   if(usableType == UsableType::ITEM && usableId > 0)
   {
      serializedShortcut = Json::Value(Json::objectValue);
      serializedShortcut[ID_ATTRIBUTE] = usableId;
      serializedShortcut[TYPE_ATTRIBUTE] = static_cast<int>(usableType);
   }
   else if(usableType == UsableType::SKILL && usableId > 0 && !characterId.empty())
   {
      serializedShortcut = Json::Value(Json::objectValue);
      serializedShortcut[Shortcut::ID_ATTRIBUTE] = usableId;
      serializedShortcut[Shortcut::TYPE_ATTRIBUTE] = static_cast<int>(usableType);
      serializedShortcut[Shortcut::CHARACTER_ID_ATTRIBUTE] = characterId;
   }

   return serializedShortcut;
}

