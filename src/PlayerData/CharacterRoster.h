/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef CHARACTER_ROSTER_H
#define CHARACTER_ROSTER_H

#include <map>
#include <vector>

namespace Json
{
   class Value;
};

class Character;

class CharacterRoster
{
   /** The lead character in the party, who the player sees when they are playing the game. */
   Character* partyLeader;

   /** The characters in the main party, who interact with the world and participate in combat. */
   std::vector<Character*> party;

   /**
    * A list of all playable characters encountered.
    * This includes characters in the party, but also includes characters encountered by the player,
    * but currently inaccessible for play.
    */
   std::map<std::string, Character*> allCharacters;

   public:
      CharacterRoster();
      ~CharacterRoster();
      void load(Json::Value& charactersElement);

      /**
       * Serializes the character data and party layout into JSON.
       *
       * @return The serialized JSON for this roster.
       */
      Json::Value serialize() const;

      Character* getPartyLeader() const;
      Character* getCharacter(const std::string& characterName) const;
      const std::vector<Character*>& getParty() const;
      void addToParty(Character* character);
      void removeFromParty(Character* character);
};

#endif
