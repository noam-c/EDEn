/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef TILE_STATE_H
#define TILE_STATE_H

#ifndef NULL
   #define NULL 0
#endif

/**
 * The state of a tile, specifically the kind of entity occupying it and
 * a pointer to that entity.
 */
struct TileState
{
   /**
    * The kinds of entities that can occupy a tile.
    */
   enum EntityType
   {
      /** No entity is on the tile */
      FREE,
      
      /** A character is on the tile */
      ACTOR,
      
      /** An obstacle (or impassable terrain) is on the tile */
      OBSTACLE,
   };
   
   /** The type of entity in the tile. */
   EntityType entityType;
   
   /** The entity in the tile. */
   void* entity;
   
   /**
    * Constructor.
    *
    * @param type The type of entity occupying the tile.
    * @param entity The entity in the tile.
    */
   TileState(EntityType type = FREE, void* entity = NULL);
};

#endif
