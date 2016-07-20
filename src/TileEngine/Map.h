/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef MAP_H
#define MAP_H

#include "tinyxml.h"

#include <fstream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Direction.h"
#include "Grid.h"
#include "MapExit.h"
#include "Point2D.h"
#include "Rectangle.h"
#include "Size.h"
#include "TriggerZone.h"

class Layer;

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
   public:
      struct NPCSpawnPoint
      {
         std::string name;
         std::string spritesheet;
         geometry::Point2D location;
         geometry::Size size;
         geometry::Direction direction;
         
         NPCSpawnPoint(const std::string& name, const std::string& spritesheet, geometry::Point2D location, geometry::Size size, geometry::Direction direction);
      };
      
   private:
      /** The name of this map */
      std::string m_name;

      /** Background layers, which are drawn behind the sprite layer (behind NPCs, player, etc.) */
      std::vector<std::unique_ptr<Layer>> m_backgroundLayers;

      /** Foreground layers, which are drawn in front of the sprite layer (in front of NPCs, player, etc.) */
      std::vector<std::unique_ptr<Layer>> m_foregroundLayers;

      /** The passibility of the map. Typed as int to avoid vector<bool> specialization. */
      Grid<int> m_passibilityMap;

      /** The list of the map's trigger zones */
      std::vector<TriggerZone> m_triggerZones;

      /** The list of the map's entrances */
      std::map<std::string, geometry::Point2D> m_mapEntrances;

      /** The list of the map's exits */
      std::vector<MapExit> m_mapExits;
      
      std::vector<NPCSpawnPoint> m_npcsToSpawn;

      /** The bounds (in tiles) of this map */
      geometry::Rectangle m_bounds;

      /**
       * Adds an collision rectangle to the passibility map, marking
       * the area occupied by the rectangle as impassible.
       *
       * @param rect The map subregion that should be marked impassible
       */
      void addCollisionRect(const geometry::Rectangle& rect);
      
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
       * Parse the map layer that holds map NPC data.
       */
      void parseNPCGroup(const TiXmlElement* triggersGroupElement);
      
      /**
       * Creates a 2-dimensional map that corresponds to the passibility of this Map
       */
      void initializePassibilityMatrix();

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
       *
       * Explicitly declared and defaulted in order to force the definition
       * out of the header file (tiny build-time optimization).
       */
      ~Map();

      /**
       * @return The name of this map.
       */
      const std::string& getName() const;

      /**
       * @return The bounds of the map (in tiles).
       */
      const geometry::Rectangle& getBounds() const;

      /**
       * @return The list of trigger zones for this map
       */
      const std::vector<TriggerZone>& getTriggerZones() const;

      /**
       * @param previousMap The name of the map that the player is entering this map from.
       *
       * @return The point of entry for the given origin map name.
       */
      const geometry::Point2D& getMapEntrance(const std::string& previousMap) const;

      /**
       * @return The list of exits for this map
       */
      const std::vector<MapExit>& getMapExits() const;

      /**
       * @return The list of NPCs to spawn for this map
       */
      const std::vector<NPCSpawnPoint>& getNPCSpawnPoints() const;
   
      /**
       * @return true iff the tile at this location of the map is passible
       */
      bool isPassible(int x, int y) const;

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
