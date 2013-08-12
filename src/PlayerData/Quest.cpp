/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "Quest.h"
#include "json.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_PLAYER;

const char* Quest::NAME_ATTRIBUTE = "name";

const char* Quest::QUEST_ELEMENT = "Quest";
const char* Quest::DESCRIPTION_ELEMENT = "Description";
const char* Quest::COMPLETED_ATTRIBUTE = "completed";
const char* Quest::OPTIONAL_ATTRIBUTE = "optional";

Quest::Quest(const std::string& name, const std::string& description, bool optional, bool completed) :
  m_name(name),
  m_optional(optional),
  m_completed(completed)
{
}

Quest::Quest(const Json::Value& questJson)
{
   load(questJson);
}

Quest::~Quest()
{
   for(QuestLog::iterator i  = m_subquests.begin(); i != m_subquests.end(); ++i)
   {
      delete i->second;
   }
}

void Quest::load(const Json::Value& questTree)
{
   m_subquests.clear();

   m_name = questTree[Quest::NAME_ATTRIBUTE].asString();
   const Json::Value& descriptionElement = questTree[Quest::DESCRIPTION_ELEMENT];
   m_description = descriptionElement.isString() ? descriptionElement.asString() : "";
   
   m_completed = questTree[Quest::COMPLETED_ATTRIBUTE].asBool();
   m_optional = questTree[Quest::OPTIONAL_ATTRIBUTE].asBool();

   const Json::Value& subquestNode = questTree[Quest::QUEST_ELEMENT];
   for(Json::Value::iterator iter = subquestNode.begin(); iter != subquestNode.end(); ++iter)
   {
      Quest* subquest = new Quest(*iter);
      m_subquests[subquest->getName()] = subquest;
   }
}

Json::Value Quest::serialize() const
{
   Json::Value questNode(Json::objectValue);
   questNode[Quest::NAME_ATTRIBUTE] = m_name;
   
   if(!m_description.empty())
   {
      questNode[Quest::DESCRIPTION_ELEMENT] = m_description;
   }
   
   questNode[Quest::COMPLETED_ATTRIBUTE] = m_completed;
   questNode[Quest::OPTIONAL_ATTRIBUTE] = m_optional;
   
   Json::Value subquestsNode(Json::arrayValue);
   for(QuestLog::const_iterator iter  = m_subquests.begin(); iter != m_subquests.end(); ++iter)
   {
      Json::Value subquestNode = iter->second->serialize();
      subquestsNode.append(subquestNode);
   }
   
   questNode[Quest::QUEST_ELEMENT] = subquestsNode;
   
   return questNode;
}

void Quest::addQuest(Quest* quest)
{
   DEBUG("Adding quest %s to quest %s.", quest->m_name.c_str(), m_name.c_str());
   m_subquests[quest->m_name] = quest;
}

Quest* Quest::getQuest(const std::string& questPath) const
{
   DEBUG("In quest %s, looking for subquest: %s", m_name.c_str(), questPath.c_str());
   std::string::size_type endOfRootQuest = questPath.find_first_of("/");
   if(endOfRootQuest == std::string::npos)
   {
      // Base case: the quest to find is a direct subquest (or doesn't exist at all)
      std::map<std::string, Quest*>::const_iterator subquestIter = m_subquests.find(questPath);
      if(subquestIter != m_subquests.end())
      {
         DEBUG("Found quest %s in quest %s", questPath.c_str(), m_name.c_str());
         return subquestIter->second;
      }

      // The quest does not exist
      DEBUG("Failed to find subquest: %s", questPath.c_str());
      return NULL;
   }

   // Recursive case: Need to go down the quest tree using the specified path
   std::string rootQuestName = questPath.substr(0, endOfRootQuest);
   std::string relativeQuestPath = questPath.substr(endOfRootQuest + 1);
   
   std::map<std::string, Quest*>::const_iterator subquestIter = m_subquests.find(rootQuestName);
   if(subquestIter != m_subquests.end())
   {
      return subquestIter->second->getQuest(relativeQuestPath);
   }

   // The quest tree to descend wasn't found; this quest does not exist
   DEBUG("Failed to find subquest: %s", rootQuestName.c_str());
   return NULL;
}

void Quest::complete()
{
   m_completed = true;
   m_subquests.clear();
}

bool Quest::isCompleted() const
{
   return m_completed;
}

std::string Quest::getName() const
{
   return m_name;
}

std::string Quest::getDescription() const
{
   return m_description;
}
