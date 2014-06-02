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

void Quest::load(const Json::Value& questTree)
{
   m_subquests.clear();

   m_name = questTree[Quest::NAME_ATTRIBUTE].asString();
   const Json::Value& descriptionElement = questTree[Quest::DESCRIPTION_ELEMENT];
   m_description = descriptionElement.isString() ? descriptionElement.asString() : "";
   
   m_completed = questTree[Quest::COMPLETED_ATTRIBUTE].asBool();
   m_optional = questTree[Quest::OPTIONAL_ATTRIBUTE].asBool();

   const Json::Value& subquestListNode = questTree[Quest::QUEST_ELEMENT];
   for(const auto& subquestNode : subquestListNode)
   {
      auto subquest = std::make_shared<Quest>(subquestNode);
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
   for(const auto& subquestIter : m_subquests)
   {
      Json::Value subquestNode = subquestIter.second->serialize();
      subquestsNode.append(subquestNode);
   }
   
   questNode[Quest::QUEST_ELEMENT] = subquestsNode;
   
   return questNode;
}

void Quest::addQuest(const std::shared_ptr<Quest>& quest)
{
   DEBUG("Adding quest %s to quest %s.", quest->m_name.c_str(), m_name.c_str());
   m_subquests.emplace(std::piecewise_construct,
                       std::forward_as_tuple(quest->m_name),
                       std::forward_as_tuple(quest));
}

const std::weak_ptr<Quest> Quest::getQuest(const std::string& questPath) const
{
   DEBUG("In quest %s, looking for subquest: %s", m_name.c_str(), questPath.c_str());
   std::string::size_type endOfRootQuest = questPath.find_first_of("/");
   if(endOfRootQuest == std::string::npos)
   {
      // Base case: the quest to find is a direct subquest (or doesn't exist at all)
      const auto& subquestIter = m_subquests.find(questPath);
      if(subquestIter != m_subquests.end())
      {
         DEBUG("Found quest %s in quest %s", questPath.c_str(), m_name.c_str());
         return subquestIter->second;
      }

      // The quest does not exist
      DEBUG("Failed to find subquest: %s", questPath.c_str());
      return std::weak_ptr<Quest>();
   }

   // Recursive case: Need to go down the quest tree using the specified path
   std::string rootQuestName = questPath.substr(0, endOfRootQuest);
   std::string relativeQuestPath = questPath.substr(endOfRootQuest + 1);
   
   const auto& subquestIter = m_subquests.find(rootQuestName);
   if(subquestIter != m_subquests.end())
   {
      return subquestIter->second->getQuest(relativeQuestPath);
   }

   // The quest tree to descend wasn't found; this quest does not exist
   DEBUG("Failed to find subquest: %s", rootQuestName.c_str());
   return std::weak_ptr<Quest>();
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

bool Quest::isStarted(const std::string& questPath) const
{
   return !getQuest(questPath).expired();
}

bool Quest::isCompleted(const std::string& questPath) const
{
   const auto& subquest = getQuest(questPath).lock();
   return subquest && subquest->isCompleted();
}

std::string Quest::getName() const
{
   return m_name;
}

std::string Quest::getDescription() const
{
   return m_description;
}
