#ifndef __MAP_H_
#define __MAP_H_

#include <fstream>
#include <string>

class Pathfinder;
class Tileset;

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

   /** Pathfinder used to find paths through this map */
   Pathfinder* pathfinder;

   /** The tiles of the map (as numbers referring to points in the Tileset) */
   int** tileMap;

   /** The passibility of the map */
   bool** passibilityMap;

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

   public:
      /**
       * Copy constructor.
       *
       * @param map The Map to copy.
       */
      Map(const Map& map);

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
       * @return The path finder for this map
       */
      Pathfinder* getPathfinder() const;

      /**
       * @return The passibility map of the 
       */
      bool** getPassibilityMatrix() const;

      /**
       * @return A copy of the map.
       */
      Map* makeCopy();

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
