/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "Quest.h"
#include "SaveGameItemNames.h"
#include "json.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_PLAYER;

Quest::Quest(const std::string& name, const std::string& description, bool optional, bool completed)
   : name(name), optional(optional), completed(completed)
{
}

Quest::Quest(const Json::Value& questJson)
{
   load(questJson);
}

Quest::~Quest()
{
   for(QuestLog::iterator i  = subquests.begin(); i != subquests.end(); ++i)
   {
      delete i->second;
   }
}

void Quest::load(const Json::Value& questTree)
{
   subquests.clear();

   name = questTree[NAME_ATTRIBUTE].asString();
   const Json::Value& descriptionElement = questTree[DESCRIPTION_ELEMENT];
   description = descriptionElement.isString() ? descriptionElement.asString() : "";
   
   completed = questTree[COMPLETED_ATTRIBUTE].asBool();
   optional = questTree[OPTIONAL_ATTRIBUTE].asBool();

   const Json::Value& subquestNode = questTree[QUEST_ELEMENT];
   for(Json::Value::iterator iter = subquestNode.begin(); iter != subquestNode.end(); ++iter)
   {
      Quest* subquest = new Quest(*iter);
      subquests[subquest->getName()] = subquest;
   }
}

Json::Value Quest::serialize() const
{
   Json::Value questNode(Json::objectValue);
   questNode[NAME_ATTRIBUTE] = name;
   
   if(!description.empty())
   {
      questNode[DESCRIPTION_ELEMENT] = description;
   }
   
   questNode[COMPLETED_ATTRIBUTE] = completed;
   questNode[OPTIONAL_ATTRIBUTE] = optional;
   
   Json::Value subquestsNode(Json::arrayValue);
   for(QuestLog::const_iterator iter  = subquests.begin(); iter != subquests.end(); ++iter)
   {
      Json::Value subquestNode = iter->second->serialize();
      subquestsNode.append(subquestNode);
   }
   
   questNode[QUEST_ELEMENT] = subquestsNode;
   
   return questNode;
}

void Quest::addQuest(Quest* quest)
{
   DEBUG("Adding quest %s to quest %s.", quest->name.c_str(), name.c_str());
   subquests[quest->name] = quest;
}

Quest* Quest::getQuest(const std::string& questPath) const
{
   DEBUG("In quest %s, looking for subquest: %s", name.c_str(), questPath.c_str());
   std::string::size_type endOfRootQuest = questPath.find_first_of("/");
   if(endOfRootQuest == std::string::npos)
   {
      // Base case: the quest to find is a direct subquest (or doesn't exist at all)
      std::map<std::string, Quest*>::const_iterator subquestIter = subquests.find(questPath);
      if(subquestIter != subquests.end())
      {
         DEBUG("Found quest %s in quest %s", questPath.c_str(), name.c_str());
         return subquestIter->second;
      }

      // The quest does not exist
      DEBUG("Failed to find subquest: %s", questPath.c_str());
      return NULL;
   }

   // Recursive case: Need to go down the quest tree using the specified path
   std::string rootQuestName = questPath.substr(0, endOfRootQuest);
   std::string relativeQuestPath = questPath.substr(endOfRootQuest + 1);
   
   std::map<std::string, Quest*>::const_iterator subquestIter = subquests.find(rootQuestName);
   if(subquestIter != subquests.end())
   {
      return subquestIter->second->getQuest(relativeQuestPath);
   }

   // The quest tree to descend wasn't found; this quest does not exist
   DEBUG("Failed to find subquest: %s", rootQuestName.c_str());
   return NULL;
}

void Quest::complete()
{
   completed = true;
   subquests.clear();
}

bool Quest::isCompleted() const
{
   return completed;
}

std::string Quest::getName() const
{
   return name;
}

std::string Quest::getDescription() const
{
   return description;
}
