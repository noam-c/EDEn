/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef COMBAT_ACTOR_H
#define COMBAT_ACTOR_H

#include "Actor.h"

class CombatActor : public Actor
{
   public:
      /**
       * Constructor for the actor.
       *
       * @param name The name of the actor (must also be the name of its script).
       * @param location The location (in pixels) where the actor will start off.
       * @param size The size (in pixels) of the actor.
       * @param direction The starting direction of the actor.
       */
      CombatActor(const std::string& name, const geometry::Point2D& location, const geometry::Size& size, geometry::Direction direction);

      CombatActor(const CombatActor&) = delete;
      CombatActor& operator=(const CombatActor&) = delete;

      CombatActor(CombatActor&&);
      CombatActor& operator=(CombatActor&&);

      ~CombatActor();
};

#endif
