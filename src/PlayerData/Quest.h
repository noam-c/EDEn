#ifndef QUEST_H
#define QUEST_H

#include <string>
#include <map>

namespace Json
{
   class Value;
};

class Quest
{
   typedef std::map<std::string, Quest*> QuestLog;

   std::string name;
   std::string description;
   bool completed;
   bool optional;

   QuestLog subquests;

   Quest* findQuest(const std::string& questPath);

   public:
      Quest(const std::string& name, const std::string& description, bool optional, bool completed);
      Quest(Json::Value& questTree);
      void serialize(Json::Value& outputXml);

      void addQuest(const std::string& questPath, const std::string& description, bool optional, bool completed = false);
      bool isCompleted();
      void complete();
      bool isQuestCompleted(const std::string& questPath);
      void completeQuest(const std::string& questPath);

      std::string getName();
      std::string getDescription();
      std::string getQuestDescription(const std::string& questPath);

      ~Quest();
};

#endif
