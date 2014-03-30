/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef ASPECT_H
#define ASPECT_H

namespace Json
{
   class Value;
};

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "UsableId.h"

/**
 * An 'Aspect' represents a facet of a character
 * that can be expressed. Each character has a
 * variety of Aspects that emphasize different
 * strengths, weaknesses and skills.
 *
 * When an Aspect is selected, the Character
 * immediately receives the stat bonuses granted
 * by the Aspect, scaled to the character level.
 *
 * In the future, Aspects will also determine
 * the skill trees that the Character can learn.
 *
 * @author Noam Chitayat
 */
class Aspect
{
   static const char* ID_ATTRIBUTE;
   static const char* NAME_ATTRIBUTE;
   static const char* STATS_ELEMENT;
   static const char* SKILLS_ELEMENT;
   static const char* PREREQUISITES_ELEMENT;

   /**
    * A helper class to encapsulate the
    * calculation of an Aspect's stat bonus
    * given a Character's level.
    *
    * @author Noam Chitayat
    */
   class StatBonusCalculation
   {
      /** The coefficient by which the stat bonus will be scaled */
      double m_coefficient;

      public:
         /**
          * Constructor.
          *
          * @param statBonusNode The JSON node representing the stat bonus.
          */
         StatBonusCalculation(const Json::Value& statBonusNode);

         /**
          * Destructor.
          */
         ~StatBonusCalculation();

         /**
          * @param level The level to which the stat bonus should scale.
          *
          * @return The calculated stat bonus proportional to the given level.
          */
         int getBonusForLevel(unsigned int level) const;
   };
   
   /** The unique ID of the aspect. */
   std::string m_id;
   
   /** The displayed name of the aspect. */
   std::string m_name;
   
   /** A list of prerequisite skills. */
   typedef std::vector<UsableId> PrerequisiteList;
   
   /** A node containing a skill ID and its prerequisites. */
   typedef std::pair<UsableId, PrerequisiteList> SkillNode;
   
   /** A directed graph of skills, where each skill is connected to its prerequisites */
   typedef std::vector<SkillNode> SkillGraph;
   
   /**
    * A comparator used to match a SkillNode to a UsableId.
    *
    * @author Noam Chitayat
    */
   struct CompareUsableId
   {
      /**
       * Comparison operation between a SkillNode and a UsableId.
       *
       * @param value An ID to compare against the node's ID.
       * @param node A Skill node to check.
       *
       * @return true iff the SkillNode's ID is less than value.
       */
      bool operator()(const UsableId value, const Aspect::SkillNode& node) const;

      /**
       * Comparison operation between a SkillNode and a UsableId.
       *
       * @param node A Skill node to check.
       * @param value An ID to compare against the node's ID.
       *
       * @return true iff the SkillNode's ID is less than value.
       */
      bool operator()(const Aspect::SkillNode& node, const UsableId value) const;
};

   /** The skill tree for the Aspect's skills and prerequisites */
   SkillGraph m_skillTree;
   
   /** The set of stat attribute bonuses granted by the Aspect. */
   std::map<std::string, StatBonusCalculation> m_statBonusCalculations;
   
   /**
    * Constructor.
    *
    * @param aspectToLoad The JSON node representing the Aspect.
    */
   Aspect(const Json::Value& aspectToLoad);
   
   /**
    * Loads the stat bonuses for the aspect.
    *
    * @param aspectToLoad The JSON node representing the Aspect.
    */
   void parseStatBonuses(const Json::Value& aspectToLoad);

   /**
    * Loads the skill tree for the aspect.
    *
    * @param aspectToLoad The JSON node representing the Aspect.
    */
   void parseSkillTree(const Json::Value& aspectToLoad);

   /**
    * Validates that the Aspect's skill tree is well-formed
    */
   void validateSkillTree() const;
   
   public:
      /**
       * Loads the aspect data using the given ID.
       *
       * @param aspectId The ID of the aspect to load.
       *
       * @return The requested Aspect or nullptr if it is not found.
       */
      static std::unique_ptr<Aspect> loadAspect(const std::string& aspectId);

      /**
       * Destructor.
       */
      ~Aspect();

      /**
       * @return the ID of this aspect.
       */
      std::string getId() const;

      /**
       * Gets the bonus granted for a given attribute.
       *
       * @param stat The name of the stat for which bonus will be calculated.
       * @param level The level to which the stat bonus should scale.
       *
       * @return The bonus granted to the attribute, proportional to the given level.
       */
      int getAspectBonus(const std::string& stat, unsigned int level) const;
   
      std::vector<UsableId> getAvailableSkills(const std::vector<UsableId>& adeptSkills) const;
};

#endif

