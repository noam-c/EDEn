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

/**
 * The exact location where the game was last saved, for reloading game state.
 *
 * @author Noam Chitayat
 */
struct SaveLocation
{
   std::string region;
   std::string map;
   int x;
   int y;
};

/**
 * A model representing the player's game data, which includes
 * character data, inventory, quest log, and game state.
 *
 * @author Noam Chitayat
 */
class PlayerData
{
   /**
    * A list of all playable characters encountered.
    * This includes characters in the party and the reserve,
    * but also includes characters encountered by the player,
    * but currently inaccessible for play.
    */
   CharacterList charactersEncountered;

   /** The characters in the main party, who interact with the world and participate in combat. */
   CharacterList party;
   
   /** The characters who are available to switch into the party. */
   CharacterList reserve;

   /** The lead character in the party, who the player sees when they are playing the game. */
   Character* partyLeader;

   /** All the items that are in the player's item bag. Includes usables, keys, and unused equipment. */
   ItemList inventory;

   /** The top-level quest for the game. Contains all the quests that the player can complete. */
   Quest* rootQuest;

   /** The current chapter being played (if the game was saved in the middle of a chapter. */
   std::string currChapter;

   /** The location of the last save point used. */
   SaveLocation saveLocation;
   
   void parseCharactersAndParty(Json::Value& rootElement);
   void serializeCharactersAndParty(Json::Value& outputXml);
    
   void parseQuestLog(Json::Value& rootElement);
   void serializeQuestLog(Json::Value& outputXml);
    
   void parseInventory(Json::Value& rootElement);
   void serializeInventory(Json::Value& outputXml);
    
   void parseLocation(Json::Value& rootElement);
   void serializeLocation(Json::Value& outputXml);
    
   public:
      static const int PARTY_SIZE = 4;
   
      /**
       * Constructor.
       */
      PlayerData();
   
      /**
       * Load the player data from a file.
       *
       * @param filePath The path to load the player data from.
       */
      void load(const std::string& filePath);

      /**
       * Save the player data to a file.
       *
       * @param filePath The path to save the player data to.
       */
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
