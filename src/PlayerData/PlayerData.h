#ifndef __PLAYER_DATA_H_
#define __PLAYER_DATA_H_

#include <map>

class Character;
class Quest;
class TiXmlElement;

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
   Character* partyLeader;

   ItemList inventory;

   Quest* rootQuest;

   std::string currChapter;

   void parseCharactersAndParty(TiXmlElement* rootElement);
   void serializeCharactersAndParty(TiXmlElement& outputXml);

   void parseQuestLog(TiXmlElement* rootElement);
   void serializeQuestLog(TiXmlElement& outputXml);

   void parseInventory(TiXmlElement* rootElement);
   void serializeInventory(TiXmlElement& outputXml);

   void parseLocation(TiXmlElement* rootElement);
   void serializeLocation(TiXmlElement& outputXml);

   public:
      void load(const std::string& filePath);
      void save(const std::string& filePath);

      void addNewCharacter(Character* newCharacter);
      Character* getPartyLeader();

      void addNewQuest(const std::string& questPath, const std::string& description, bool optionalQuest);
      bool isQuestCompleted(const std::string& questPath);
      void completeQuest(const std::string& questPath);
      std::string getQuestDescription(const std::string& questPath);

      ~PlayerData();
};

#endif