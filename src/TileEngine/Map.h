/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#ifndef MAP_H
#define MAP_H

#include <fstream>
#include <string>
#include <vector>

class Tileset;
class Obstacle;

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
   protected:
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

      /** The list of the map's obstacles */
      std::vector<Obstacle*> obstacles;

      /** Width (in tiles) of this map */
      int width;

      /** Height (in tiles) of this map */
      int height;

      /**
       * @return true iff the tile at this location of the map is passible
       */
      bool isPassible(int x, int y) const;

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
       * @param in The region file stream, currently pointing at the
       *           beginning of this Map's data.
       */
      Map(std::ifstream& in);

      /**
       * @return The name of this map.
       */
      std::string getName() const;

      /**
       * @return The width of the map (in tiles).
       */
      int getWidth() const;

      /**
       * @return The height of the map (in tiles).
       */
      int getHeight() const;

      /**
       * @return The list of obstacles for this map
       */
      const std::vector<Obstacle*> getObstacles() const;

      /**
       * @return The passibility map of the 
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
