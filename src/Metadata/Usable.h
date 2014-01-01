/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef USABLE_H
#define USABLE_H

#include <string>
#include "UsableId.h"

/**
 * \todo Change this include to forward declarations when enums
 * can be forward declared in the product.
 */
#include "GameState.h"

namespace Json
{
   class Value;
};

class ScriptEngine;
class UsableScript;
class Character;

/**
 * Metadata for a usable. Since currently, all the games usables are non-customizable (no 'unique' usables),
 * a template is sufficient for describing all the properties that an usable will have. As a result,
 * Usable is not an object representative of a single usable found in the world; it is the information describing a given usable.
 */
class Usable
{
   static const char* ID_ATTRIBUTE;
   static const char* NAME_ATTRIBUTE;
   static const char* ICON_ATTRIBUTE;

   /** The unique identifier of this usable. */
   const UsableId m_id;
   
   /** The name of this usable. */
   const std::string m_name;

   /** The path to the icon use to represent this usable visually. */
   const std::string m_iconPath;

   /** The script that dictates what the usable does when used. */
   UsableScript* m_usableScript;

   void loadScript(ScriptEngine& scriptEngine);

   protected:
      virtual UsableScript* createScript(ScriptEngine& scriptEngine);

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
      virtual ~Usable();

      /**
       * @return The unique identifier of this usable.
       */
      UsableId getId() const;
      
      /**
       * @return The name of this usable.
       */
      const std::string& getName() const;

      /**
       * @return The icon visually representing this usable.
       */
      const std::string& getIconPath() const;

      virtual bool use(ScriptEngine& scriptEngine, GameState::GameStateType gameStateType, Character* usingCharacter = NULL);
};

#endif
