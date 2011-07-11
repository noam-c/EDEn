#ifndef QUEST_H
#define QUEST_H

#include <string>
#include <map>

namespace Json
{
   class Value;
};

/**
 * \todo Document.
 */
class Quest
{
   /**
    * \todo Document.
    */
   typedef std::map<std::string, Quest*> QuestLog;

   
   /**
    * \todo Document.
    */
   std::string name;
   
   /**
    * \todo Document.
    */
   std::string description;
   
   /**
    * \todo Document.
    */
   bool completed;
   
   /**
    * \todo Document.
    */
   bool optional;

   
   /**
    * \todo Document.
    */
   QuestLog subquests;

   
   /**
    * \todo Document.
    */
   Quest* findQuest(const std::string& questPath);

   public:   
      /**
       * \todo Document.
       */
      Quest(const std::string& name, const std::string& description, bool optional, bool completed);
   
      /**
       * \todo Document.
       */
      Quest(Json::Value& questTree);
   
      /**
       * \todo Document.
       */
      void serialize(Json::Value& outputXml);

   
      /**
       * \todo Document.
       */
      void addQuest(const std::string& questPath, const std::string& description, bool optional, bool completed = false);
   
      /**
       * \todo Document.
       */
      bool isCompleted();
   
      /**
       * \todo Document.
       */
      void complete();
   
      /**
       * \todo Document.
       */
      bool isQuestCompleted(const std::string& questPath);
   
      /**
       * \todo Document.
       */
      void completeQuest(const std::string& questPath);

   
      /**
       * \todo Document.
       */
      std::string getName();
   
      /**
       * \todo Document.
       */
      std::string getDescription();
   
      /**
       * \todo Document.
       */
      std::string getQuestDescription(const std::string& questPath);

   
      /**
       * \todo Document.
       */
      ~Quest();
};

#endif
