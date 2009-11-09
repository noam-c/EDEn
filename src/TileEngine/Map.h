#ifndef __MAP_H_
#define __MAP_H_

#include "Tileset.h"
#include <fstream>
#include <string>

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

   /** The tiles of the map (as numbers referring to points in the Tileset */
   int** tileMap;

   /** Width (in tiles) of this map */
   int width;

   /** Height (in tiles) of this map */
   int height;

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
      std::string getName();

      /**
       * @return The width of the map (in tiles).
       */
      int getWidth();

      /**
       * @return The height of the map (in tiles).
       */
      int getHeight();

      /**
       * Draw an entire map's tiles.
       */
      void draw();

      /**
       * Destructor.
       */
      ~Map();
};

#endif
