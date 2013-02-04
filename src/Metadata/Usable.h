/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef USABLE_H
#define USABLE_H

#include <string>

namespace Json
{
   class Value;
};

class GameContext;
class UsableScript;

/**
 * Metadata for an usable. Since currently, all the games usables are non-customizable (no 'unique' usables),
 * a template is sufficient for describing all the properties that an usable will have. As a result,
 * Usable is not an object representative of a single usable found in the world; it is the information describing a given usable.
 */
class Usable
{
   /** The unique identifier of this usable. */
   const int id;
   
   /** The name of this usable. */
   const std::string name;

   /** The path to the icon use to represent this usable visually. */
   const std::string iconPath;

   /** The script that dictates what the usable does when used. */
   UsableScript* usableScript;

   public:
      /**
       * Constructor.
       *
       * @param usableNode The JSON node containing the usable metadata to load.
       */
      Usable(Json::Value& usableNode);
   
      /**
       * Destructor.
       */
      ~Usable();

      /**
       * @return The unique identifier of this usable.
       */
      const int getId() const;
      
      /**
       * @return The name of this usable.
       */
      const std::string& getName() const;

      /**
       * @return The icon visually representing this usable.
       */
      const std::string& getIconPath() const;

      void loadScript(GameContext& gameContext);

      bool use(GameContext& gameContext);
};

#endif
