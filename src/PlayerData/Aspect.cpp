#include "Aspect.h"
#include <algorithm>
#include <fstream>
#include <cmath>
#include <queue>
#include <map>
#include "json.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_PLAYER;

const char* Aspect::ID_ATTRIBUTE = "id";
const char* Aspect::NAME_ATTRIBUTE = "name";
const char* Aspect::STATS_ELEMENT = "stats";
const char* Aspect::SKILLS_ELEMENT = "skills";
const char* Aspect::PREREQUISITES_ELEMENT = "requires";

Aspect::StatBonusCalculation::StatBonusCalculation(const Json::Value& statBonusNode) :
   m_coefficient(std::max<double>(statBonusNode.asDouble(), 1))
{
}

Aspect::StatBonusCalculation::~StatBonusCalculation()
{
}

int Aspect::StatBonusCalculation::getBonusForLevel(unsigned int level) const
{
   // ceil instead of round because we <3 players
   return ceil(level * m_coefficient);
}

Aspect* Aspect::loadAspect(const std::string& aspectId)
{
   const std::string path = std::string("data/aspects/") + aspectId + ".eda";
   DEBUG("Loading aspect %s in file %s", aspectId.c_str(), path.c_str());
   
   std::ifstream input(path.c_str());
   if(!input)
   {
      T_T("Failed to open aspect file for reading.");
   }
   
   Json::Value jsonRoot;
   input >> jsonRoot;
   
   if(jsonRoot.isNull())
   {
      DEBUG("No root element was found.");
      T_T("Failed to parse aspect data.");
   }
   
   return new Aspect(jsonRoot);
}

Aspect::Aspect(const Json::Value& aspectToLoad)
{
   m_id = aspectToLoad[Aspect::ID_ATTRIBUTE].asString();
   m_name = aspectToLoad[Aspect::NAME_ATTRIBUTE].asString();
   
   parseStatBonuses(aspectToLoad);
   parseSkillTree(aspectToLoad);
}

Aspect::~Aspect()
{
}

void Aspect::parseStatBonuses(const Json::Value& aspectToLoad)
{
   const Json::Value& statBonuses = aspectToLoad[Aspect::STATS_ELEMENT];
   for(Json::Value::const_iterator iter = statBonuses.begin(); iter != statBonuses.end(); ++iter)
   {
      std::pair<std::string, StatBonusCalculation> statCalculation(iter.key().asString(), *iter);
      m_statBonusCalculations.insert(statCalculation);
   }
}

void Aspect::parseSkillTree(const Json::Value& aspectToLoad)
{
   const Json::Value& skills = aspectToLoad[Aspect::SKILLS_ELEMENT];
   if(skills.isArray())
   {
      for(Json::Value::const_iterator iter = skills.begin(); iter != skills.end(); ++iter)
      {
         UsableId skillId = (*iter)[Aspect::ID_ATTRIBUTE].asUInt();
         const Json::Value& prereqsNode = (*iter)[Aspect::PREREQUISITES_ELEMENT];
         
         PrerequisiteList prereqs;
         
         if(prereqsNode.isArray())
         {
            for(Json::Value::const_iterator iter = prereqsNode.begin(); iter != prereqsNode.end(); ++iter)
            {
               UsableId prereqId = (*iter).asUInt();
               PrerequisiteList::iterator insertionPoint = std::lower_bound(prereqs.begin(), prereqs.end(), prereqId);
               prereqs.insert(insertionPoint, prereqId);
            }
         }

         std::pair<UsableId, PrerequisiteList> skillAndPrereqs(skillId, prereqs);
         m_skillTree.push_back(skillAndPrereqs);
      }

      validateSkillTree();
   }
}

void Aspect::validateSkillTree() const
{
   const int numSkills = m_skillTree.size();

   std::map<UsableId, int> skillTreeMap;
   
   // Validate that there aren't any duplicates and build
   // a map for the skills
   // \todo Can this function be implemented efficiently without a map?
   for(int i = 0; i < numSkills; ++i)
   {
      UsableId id = m_skillTree[i].first;
      if(skillTreeMap.find(id) != skillTreeMap.end())
      {
         T_T("Skill tree for aspect has a duplicated skill");
      }
      
      skillTreeMap[id] = i;
   }
   
   // Validate that there aren't any prerequisite cycles
   int currentColor = 0;
   std::vector<int> traversedColor(numSkills, -1);

   std::queue<int> bfsQueue;

   for(int i = 0; i < numSkills; ++i)
   {
      // If we've already seen this node, searched
      // all its prerequisite chains and
      // haven't thrown, then it can't lead to a cycle
      if(traversedColor[i] >= 0) continue;

      // Perform a standard breadth-first search from
      // this node
      bfsQueue.push(i);
      while(!bfsQueue.empty())
      {
         int currentNode = bfsQueue.front();
         bfsQueue.pop();
         
         if(traversedColor[currentNode] >= 0)
         {
            if(traversedColor[currentNode] == currentColor)
            {
               // If the color of the node is the current color,
               // then the breadth-first search has found a cycle
               T_T("Skill tree for aspect has a cycle");
            }
         }
         else
         {
            // Color the node with the current color
            traversedColor[currentNode] = currentColor;
         }
         
         // Push all of this node's prerequisites into the breadth-first search queue
         PrerequisiteList prereqs = m_skillTree[currentNode].second;
         for(PrerequisiteList::const_iterator iter = prereqs.begin(); iter != prereqs.end(); ++iter)
         {
            std::map<UsableId, int>::const_iterator skillNode = skillTreeMap.find(*iter);
            if(skillNode == skillTreeMap.end())
            {
               T_T("One of the skills in this aspect's tree has a prerequisite that isn't in the tree.")
            }

            bfsQueue.push(skillNode->second);
         }
      }

      ++currentColor;
   }
}

std::string Aspect::getId() const
{
   return m_id;
}

int Aspect::getAspectBonus(const std::string& stat, unsigned int level) const
{
   std::map<std::string, StatBonusCalculation>::const_iterator iter = m_statBonusCalculations.find(stat);
   if (iter != m_statBonusCalculations.end())
   {
      return iter->second.getBonusForLevel(level);
   }
   
   return 0;
}

std::vector<UsableId> Aspect::getAvailableSkills(const std::vector<UsableId>& adeptSkills) const
{
   std::vector<UsableId> availableSkills;
   for(std::vector<std::pair<UsableId, PrerequisiteList> >::const_iterator iter = m_skillTree.begin(); iter != m_skillTree.end(); ++iter)
   {
      bool skillAlreadyAdept = std::binary_search(adeptSkills.begin(), adeptSkills.end(), iter->first);
      if(skillAlreadyAdept)
      {
         continue;
      }

      PrerequisiteList prerequisites = iter->second;
      if(std::includes(adeptSkills.begin(), adeptSkills.end(), prerequisites.begin(), prerequisites.end()))
      {
         availableSkills.push_back(iter->first);
      }
   }
   
   return availableSkills;
}
