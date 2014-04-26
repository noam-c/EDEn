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

std::unique_ptr<Aspect> Aspect::loadAspect(const std::string& aspectId)
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
   
   return std::unique_ptr<Aspect>(new Aspect(jsonRoot));
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
      m_statBonusCalculations.emplace(iter.key().asString(), *iter);
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
         SkillGraph::iterator insertionPoint = std::lower_bound(m_skillTree.begin(), m_skillTree.end(), skillAndPrereqs);
         m_skillTree.insert(insertionPoint, skillAndPrereqs);
      }

      validateSkillTree();
   }
}

void Aspect::validateSkillTree() const
{
   if(!std::is_sorted(
       m_skillTree.begin(),
       m_skillTree.end(),
       [](SkillNode left, SkillNode right)
       {
          return left.first < right.first;
       }))
   {
      T_T("Expected skill tree to be sorted by Skill ID");
   }

      const int numSkills = m_skillTree.size();

   // Validate that there aren't any duplicates
   for(int i = 1; i < numSkills; ++i)
   {
      if(m_skillTree[i - 1].first == m_skillTree[i].first)
      {
         T_T("Skill tree for aspect has a duplicated skill");
      }
   }
   
   // Validate that there aren't any prerequisite cycles
   // by using graph coloring
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
         for(const auto& prereq : prereqs)
         {
            // Use std::lower_bound in order to search using
            // binary search instead of linear search
            const auto& skillNodeIter =
            std::lower_bound(m_skillTree.begin(), m_skillTree.end(), prereq, [](const SkillNode& node, const UsableId& value) {
               return node.first < value;
            });

            if((skillNodeIter == m_skillTree.end() || skillNodeIter->first != prereq))
            {
               T_T("One of the skills in this aspect's tree has a prerequisite that isn't in the tree.")
            }
            
            int indexOfPrereq = (skillNodeIter - m_skillTree.begin());
            bfsQueue.push(indexOfPrereq);
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
   auto iter = m_statBonusCalculations.find(stat);
   if (iter != m_statBonusCalculations.end())
   {
      return iter->second.getBonusForLevel(level);
   }
   
   return 0;
}

std::vector<UsableId> Aspect::getAvailableSkills(const std::vector<UsableId>& adeptSkills) const
{
   std::vector<UsableId> availableSkills;
   for(const auto& skillNode : m_skillTree)
   {
      bool skillAlreadyAdept = std::binary_search(adeptSkills.begin(), adeptSkills.end(), skillNode.first);
      if(skillAlreadyAdept)
      {
         availableSkills.push_back(skillNode.first);
      }
      else
      {
         PrerequisiteList prerequisites = skillNode.second;
         if(std::includes(adeptSkills.begin(), adeptSkills.end(), prerequisites.begin(), prerequisites.end()))
         {
            availableSkills.push_back(skillNode.first);
         }
      }
   }
   
   return availableSkills;
}
