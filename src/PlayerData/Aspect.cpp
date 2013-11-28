#include "Aspect.h"
#include <fstream>
#include <cmath>
#include "json.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_PLAYER;

const char* Aspect::ID_ELEMENT = "id";
const char* Aspect::NAME_ELEMENT = "name";
const char* Aspect::STATS_ELEMENT = "stats";

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
   m_id = aspectToLoad[Aspect::ID_ELEMENT].asString();
   m_name = aspectToLoad[Aspect::NAME_ELEMENT].asString();
   
   const Json::Value& statBonuses = aspectToLoad[Aspect::STATS_ELEMENT];
   for(Json::Value::const_iterator iter = statBonuses.begin(); iter != statBonuses.end(); ++iter)
   {
      std::pair<std::string, StatBonusCalculation> statCalculation(iter.key().asString(), *iter);
      m_statBonusCalculations.insert(statCalculation);
   }
}

Aspect::~Aspect()
{
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
