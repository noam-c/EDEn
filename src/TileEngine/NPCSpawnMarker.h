/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef NPC_SPAWN_MARKER_H
#define NPC_SPAWN_MARKER_H

#include <string>

#include "Direction.h"
#include "Point2D.h"
#include "Size.h"

/**
 * A marker loaded from a map indicating a type of NPC to spawn and a location
 * to spawn the NPC into.
 *
 * @author Noam Chitayat
 */
struct NPCSpawnMarker
{
   /** The name of the npc to add */
   std::string name;

   /** The name of the spritesheet to draw the NPC with */
   std::string spritesheet;

   /** npcLocation The location where we spawn the NPC */
   geometry::Point2D location;

   /** size The size of the new NPC */
   geometry::Size size;

   /** direction The direction that the new NPC will face at first */
   geometry::Direction direction;
};

#endif
