/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef MAP_H
#define MAP_H

#include "tinyxml.h"

#include <fstream>
#include <string>
#include <map>
#include <vector>

#include "Rectangle.h"

class Point2D;
class MapExit;
class Layer;
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
   std::string m_name;

   /** Background layers, which are drawn behind the sprite layer (behind NPCs, player, etc.) */
   std::vector<Layer*> m_backgroundLayers;

   /** Foreground layers, which are drawn in front of the sprite layer (in front of NPCs, player, etc.) */
   std::vector<Layer*> m_foregroundLayers;

   /** The passibility of the map */
   bool** m_passibilityMap;

   /** The list of the map's trigger zones */
   std::vector<TriggerZone> m_triggerZones;

   /** The list of the map's entrances */
   std::map<std::string, shapes::Point2D> m_mapEntrances;

   /** The list of the map's exits */
   std::vector<MapExit> m_mapExits;

   /** The bounds (in tiles) of this map */
   shapes::Rectangle m_bounds;

   /**
    * @return true iff the tile at this location of the map is passible
    */
   bool isPassible(int x, int y) const;

   /**
    * Parse the map layer that holds collision data.
    */
   void parseCollisionGroup(const TiXmlElement* collisionGroupElement);

   /**
    * Parse the map layer that holds map entrance data.
    */
   void parseMapEntrancesGroup(const TiXmlElement* entrancesGroupElement);
   
   /**
    * Parse the map layer that holds map exit data.
    */
   void parseMapExitsGroup(const TiXmlElement* exitsGroupElement);
   
   /**
    * Parse the map layer that holds map trigger zone data.
    */
   void parseMapTriggersGroup(const TiXmlElement* triggersGroupElement);

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
       * Destructor.
       */
      ~Map();

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
      const std::vector<TriggerZone>& getTriggerZones() const;

      /**
       * @param previousMap The name of the map that the player is entering this map from.
       *
       * @return The point of entry for the given origin map name.
       */
      const shapes::Point2D& getMapEntrance(const std::string& previousMap) const;

      /**
       * @return The list of exits for this map
       */
      const std::vector<MapExit>& getMapExits() const;

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
       * Draw a row of the map's background.
       *
       * @param row The row of the background to draw.
       */
      void drawBackground(int row) const;

      /**
       * Draw a row of the map's foreground.
       *
       * @param row The row of the foreground to draw.
       */
      void drawForeground(int row) const;
};

#endif
