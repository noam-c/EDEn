#include "Aspect.h"
#include <cmath>
#include "json.h"

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

Aspect::Aspect(const Json::Value& aspectToLoad)
{
   m_name = aspectToLoad[Aspect::NAME_ELEMENT].asString();
   
   const Json::Value& statBonusesNode = aspectToLoad[Aspect::STATS_ELEMENT];
   Json::Value::Members statCurves = statBonusesNode.getMemberNames();
   for(Json::Value::Members::const_iterator iter = statCurves.begin(); iter != statCurves.end(); ++iter)
   {
      const std::string& statName = *iter;
      std::pair<std::string, StatBonusCalculation> statCalculation(statName, statBonusesNode[statName]);
      m_statBonusCalculations.insert(statCalculation);
   }
}

Aspect::~Aspect()
{
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
