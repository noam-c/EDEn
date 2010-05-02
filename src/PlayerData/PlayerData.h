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
      void load(std::string filePath);
      void save(std::string filePath);

      void addNewCharacter(Character* newCharacter);
      Character* getPartyLeader();

      void addNewQuest(std::string questPath, std::string description, bool optionalQuest);
      bool isQuestCompleted(std::string questPath);
      void completeQuest(std::string questPath);
      std::string getQuestDescription(std::string questPath);

      ~PlayerData();
};

#endif