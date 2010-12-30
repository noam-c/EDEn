#include "Map.h"
#include "Tileset.h"
#include "ResourceLoader.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_RES_LOAD;

Map::Map(const Map& map) : mapName(map.mapName), tilesetName(map.tilesetName), width(map.width), height(map.height)
{
   tileset = ResourceLoader::getTileset(tilesetName);

   tileMap = new int*[height];

   for(int i = 0; i < height; ++i)
   {
      tileMap[i] = new int[width];
      for(int j = 0; j < width; ++j)
      {
         tileMap[i][j] = map.tileMap[i][j];
      }
   }

}

Map::Map(std::ifstream& in)
{
   /**
    * \todo Regions shouldn't assume that files are well-formed.
    *       Find and report errors if the file isn't formed ideally.
    */

   std::getline(in, mapName, MAP_DELIM);
   DEBUG("Loading map: %s", mapName.c_str());
   std::getline(in, tilesetName, MAP_DELIM);

   in >> width;
   in.get();
   in >> height;

   tileset = ResourceLoader::getTileset(tilesetName);

   tileMap = new int*[height];

   for(int i = 0; i < height; ++i)
   {
      tileMap[i] = new int[width];
      for(int j = 0; j < width; ++j)
      {
         if(in)
         {
            in >> tileMap[i][j];
         }
         else
         {
            T_T("Tile map incomplete.");
         }
      }

      in.ignore(width<<1, '\n');
   }

   DEBUG("Map loaded.");
}

std::string Map::getName()
{
   return mapName;
}

int Map::getWidth()
{
   return width;
}

int Map::getHeight()
{
   return height;
}

void Map::draw()
{
   for(int i = 0; i < width; ++i)
   {
      for(int j = 0; j < height; ++j)
      {
         tileset->draw(i, j, tileMap[j][i]);
      }
   }
}

Map::~Map()
{
   for(int i = 0; i < width; ++i)
   {
      delete [] tileMap[i];
   }

   delete [] tileMap;
}
