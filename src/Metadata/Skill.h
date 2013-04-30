/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef SKILL_H
#define SKILL_H

#include "Usable.h"

class Skill : public Usable
{
   protected:
      UsableScript* createScript(GameContext& gameContext);

   public:
      /**
       * Constructor.
       *
       * @param skillNode The JSON node containing the skill metadata to load.
       */
      Skill(Json::Value& skillNode);

      bool use(GameContext& gameContext, Character* usingCharacter);
};

#endif