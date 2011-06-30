#ifndef __PLAYER_DATA_H_
#define __PLAYER_DATA_H_

#include <map>
#include <string>

class Character;
class Quest;

namespace Json
{
   class Value;
};

typedef std::map<std::string, Character*> CharacterList;
typedef std::map<int, int> ItemList;

struct SaveLocation
{
   std::string region;
   std::string map;
   int x;
   int y;
};

class PlayerData
{
   CharacterList charactersEncountered;
   CharacterList party;
   CharacterList reserve;
   Character* partyLeader;

   ItemList inventory;

   Quest* rootQuest;

   std::string currChapter;

   void parseCharactersAndParty(Json::Value& rootElement);
   void serializeCharactersAndParty(Json::Value& outputXml);
    
   void parseQuestLog(Json::Value& rootElement);
   void serializeQuestLog(Json::Value& outputXml);
    
   void parseInventory(Json::Value& rootElement);
   void serializeInventory(Json::Value& outputXml);
    
   void parseLocation(Json::Value& rootElement);
   void serializeLocation(Json::Value& outputXml);
    
   public:
      void load(const std::string& filePath);
      void save(const std::string& filePath);

      void addNewCharacter(Character* newCharacter);
      Character* getPartyLeader() const;
      Character* getPartyCharacter(const std::string& characterName) const;

	   CharacterList getParty() const;
	   ItemList getInventory() const;

      void addNewQuest(const std::string& questPath, const std::string& description, bool optionalQuest);
      bool isQuestCompleted(const std::string& questPath);
      void completeQuest(const std::string& questPath);
      std::string getQuestDescription(const std::string& questPath);

      ~PlayerData();
};

#endif
