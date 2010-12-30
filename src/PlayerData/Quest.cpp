#include "Quest.h"
#include "SaveGameItemNames.h"
#include "tinyxml.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_PLAYER;

Quest::Quest(const std::string& name, const std::string& description, bool optional, bool completed)
   : name(name), optional(optional), completed(completed)
{
}

Quest::Quest(TiXmlElement* questTree)
{
   questTree->QueryStringAttribute(NAME_ATTRIBUTE, &name);

   TiXmlElement* descriptionElement = questTree->FirstChildElement(DESCRIPTION_ELEMENT);
   description = descriptionElement != NULL ? descriptionElement->GetText() : "";

   std::string completedValue;
   std::string optionalValue;

   questTree->QueryStringAttribute(COMPLETED_ATTRIBUTE, &completedValue);
   questTree->QueryStringAttribute(OPTIONAL_ATTRIBUTE, &optionalValue);

   completed = (completedValue == "true");
   optional = (optionalValue == "true");

   TiXmlElement* subquestNode = questTree->FirstChildElement(QUEST_ELEMENT);
   while(subquestNode != NULL)
   {
      Quest* subquest = new Quest(subquestNode);
      subquests[subquest->getName()] = subquest;

      subquestNode = subquestNode->NextSiblingElement(QUEST_ELEMENT);
   }
}

void Quest::serialize(TiXmlElement& outputXml)
{
   TiXmlElement questNode(QUEST_ELEMENT);
   questNode.SetAttribute(NAME_ATTRIBUTE, name);

   if(!description.empty())
   {
      TiXmlElement descriptionElement(DESCRIPTION_ELEMENT);
      TiXmlText descriptionText(description);
      descriptionElement.InsertEndChild(descriptionText);
      questNode.InsertEndChild(descriptionElement);
   }
   
   questNode.SetAttribute(COMPLETED_ATTRIBUTE, completed);
   questNode.SetAttribute(OPTIONAL_ATTRIBUTE, optional);
   for(QuestLog::iterator i  = subquests.begin(); i != subquests.end(); ++i)
   {
      i->second->serialize(questNode);
   }

   outputXml.InsertEndChild(questNode);
}

void Quest::addQuest(const std::string& questPath, const std::string& description, bool optional, bool completed)
{
   std::string::size_type endOfRootQuest = questPath.find_first_of(".");
   if(endOfRootQuest == std::string::npos)
   {
      // Base case: this is the leaf quest to add
      subquests[questPath] = new Quest(questPath, description, optional, completed);
   }
   else
   {
      // Recursive case: Need to go down the quest tree using the specified path
      std::string rootQuestName = questPath.substr(0, endOfRootQuest - 1);
      std::string relativeQuestPath = questPath.substr(endOfRootQuest);
      if(subquests.find(rootQuestName) == subquests.end())
      {
         // The quest tree to descend wasn't found; create it
         subquests[rootQuestName] = new Quest(rootQuestName, description, optional, completed);
      }

      subquests[rootQuestName]->addQuest(relativeQuestPath, description, optional, completed);
   }
}

Quest* Quest::findQuest(const std::string& questPath)
{
   std::string::size_type endOfRootQuest = questPath.find_first_of(".");
   if(endOfRootQuest == std::string::npos)
   {
      // Base case: the quest is in the subquest list (or isn't and returns NULL)
      return subquests[questPath];
   }
   else
   {
      // Recursive case: Need to go down the quest tree using the specified path
      std::string rootQuestName = questPath.substr(0, endOfRootQuest - 1);
      std::string relativeQuestPath = questPath.substr(endOfRootQuest);
      if(subquests.find(rootQuestName) == subquests.end())
      {
         // The quest tree to descend wasn't found; this quest does not exist
         return NULL;
      }

      return subquests[rootQuestName]->findQuest(relativeQuestPath);
   }
}

bool Quest::isQuestCompleted(const std::string& questPath)
{
   Quest* quest = findQuest(questPath);
   if(quest != NULL)
   {
      return quest->completed;
   }
   
   return false;
}

void Quest::completeQuest(const std::string& questPath)
{
   Quest* quest = findQuest(questPath);
   if(quest != NULL)
   {
      quest->complete();
   }
   
   /** \todo Report exception here? */
}

void Quest::complete()
{
   completed = true;
   subquests.clear();
}

bool Quest::isCompleted()
{
   return completed;
}

std::string Quest::getName()
{
   return name;
}

std::string Quest::getDescription()
{
   return description;
}

std::string Quest::getQuestDescription(const std::string& questPath)
{
   Quest* quest = findQuest(questPath);
   if(quest != NULL)
   {
      return quest->description;
   }
   
   return "";
}

Quest::~Quest()
{
   for(QuestLog::iterator i  = subquests.begin(); i != subquests.end(); ++i)
   {
      delete i->second;
   }
}
