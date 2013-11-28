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
#include <string>

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
   static const char* ID_ELEMENT;
   static const char* NAME_ELEMENT;
   static const char* STATS_ELEMENT;

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

   /** The set of stat attribute bonuses granted by the Aspect. */
   std::map<std::string, StatBonusCalculation> m_statBonusCalculations;
   
   /**
    * Constructor.
    *
    * @param aspectToLoad The JSON node representing the Aspect.
    */
   Aspect(const Json::Value& aspectToLoad);
   
   public:
      /**
       * Loads the aspect data using the given ID.
       *
       * @param aspectId The ID of the aspect to load.
       *
       * @return The requested Aspect or NULL if it is not found.
       */
      static Aspect* loadAspect(const std::string& aspectId);

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
};

#endif

