#ifndef __QUEST_H_
#define __QUEST_H_

#include <string>
#include <map>

class TiXmlElement;

class Quest
{
   typedef std::map<std::string, Quest*> QuestLog;

   std::string name;
   std::string description;
   bool completed;
   bool optional;

   QuestLog subquests;

   Quest* findQuest(std::string questPath);

   public:
      Quest(std::string name, std::string description, bool optional, bool completed);
      Quest(TiXmlElement* questTree);
      void Quest::serialize(TiXmlElement& outputXml);

      void addQuest(std::string questPath, std::string description, bool optional, bool completed = false);
      bool isCompleted();
      void complete();
      bool isQuestCompleted(std::string questPath);
      void completeQuest(std::string questPath);

      std::string getName();
      std::string getDescription();
      std::string getQuestDescription(std::string questPath);

      ~Quest();
};

#endif