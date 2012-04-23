/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef MAP_H
#define MAP_H

#include "tinyxml.h"

#include <fstream>
#include <string>
#include <vector>

#include "Rectangle.h"

class Tileset;
class Obstacle;
class TriggerZone;

/**
 * A map is a subset of a Region consisting of a single rectangular set of tiles
 * drawn using a given tileset. The player character walks around in a map,
 * interacting with NPCs and casting spells. The player may leave this map and
 * cross to either another map in the same Region, or the Overworld.
 *
 * @author Noam Chitayat
 */
class Map
{
   /**
    * The delimiter for map data read from file.
    */
   static const char MAP_DELIM = ':';

   /** The name of this map */
   std::string mapName;
 
   /** The name of the tileset in use by this map */
   std::string tilesetName;

   /** Tileset in use by this map */
   Tileset* tileset;

   /** The tiles of the map (as numbers referring to points in the Tileset) */
   int** tileMap;

   /** The passibility of the map */
   bool** passibilityMap;

   /** The list of the map's trigger zones */
   std::vector<TriggerZone*> triggerZones;
   
   /** The list of the map's obstacles */
   std::vector<Obstacle*> obstacles;

   /** The bounds (in tiles) of this map */
   shapes::Rectangle bounds;

   /**
    * @return true iff the tile at this location of the map is passible
    */
   bool isPassible(int x, int y) const;

   /**
    * Parse the map layer that holds floor tile data.
    */
   void parseFloorLayer(const TiXmlElement* floorLayerElement);

   /**
    * Parse the map layer that holds collision data.
    */
   void parseCollisionGroup(const TiXmlElement* collisionGroupElement);

   /**
    * Creates a 2-dimensional map that corresponds to the passibility of this Map
    */
   void initializePassibilityMatrix();

   /**
    * Default constructor.
    */
   Map();

   public:

      /**
       * Constructor. Loads map data from a Region file.
       * At the end of construction, the input stream 'in' will be at the end of
       * this Map's data.
       *
       * @param name The name of the map area.
       * @param filePath The path to the map file to load.
       */
      Map(const std::string& name, const std::string& filePath);

      /**
       * @return The name of this map.
       */
      const std::string& getName() const;

      /**
       * @return The bounds of the map (in tiles).
       */
      const shapes::Rectangle& getBounds() const;
      
      /**
       * @return The list of trigger zones for this map
       */
      const std::vector<TriggerZone*> getTriggerZones() const;

      /**
       * @return The list of obstacles for this map
       */
      const std::vector<Obstacle*> getObstacles() const;

      /**
       * @return The passibility of the map 
       */
      bool** getPassibilityMatrix() const;

      /**
       * Perform logic for the obstacles on the map.
       * \todo This function should be removed, since this map data should be stateless.
       */
      void step(long timePassed) const;

      /**
       * Draw an entire map's tiles.
       */
      void draw() const;

      /**
       * Destructor.
       */
      virtual ~Map();
};

#endif
