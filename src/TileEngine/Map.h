#ifndef __MAP_H_
#define __MAP_H_

#include "Tileset.h"
#include <fstream>
#include <string>

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
       Map(std::ifstream& in);
       std::string getName();
       void draw();
       ~Map();
};

#endif
