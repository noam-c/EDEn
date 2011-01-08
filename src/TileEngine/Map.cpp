#include "Map.h"
#include "Tileset.h"
#include "Pathfinder.h"
#include "ResourceLoader.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_RES_LOAD;

//#define DRAW_PASSIBILITY

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

   pathfinder = new Pathfinder(this);
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

   initializePassibilityMatrix();

   pathfinder = new Pathfinder(this);

   DEBUG("Map loaded.");
}

bool Map::isPassible(int x, int y) const
{
   return tileset->isPassible(tileMap[y][x]);
}

void Map::initializePassibilityMatrix()
{
   passibilityMap = new bool*[height];
   for(int y = 0; y < height; ++y)
   {
      passibilityMap[y] = new bool[width];
      for(int x = 0; x < width; ++x)
      {
         passibilityMap[y][x] = isPassible(x, y);
      }
   }
}

std::string Map::getName() const
{
   return mapName;
}

int Map::getWidth() const
{
   return width;
}

int Map::getHeight() const
{
   return height;
}

Pathfinder* Map::getPathfinder() const
{
   return pathfinder;
}

bool** Map::getPassibilityMatrix() const
{
   return passibilityMap;
}

void Map::draw()
{
   for(int i = 0; i < width; ++i)
   {
      for(int j = 0; j < height; ++j)
      {
#ifdef DRAW_PASSIBILITY
         if(passibilityMap[j][i])
         {
            Tileset::drawColorToTile(i, j, 0.0f, 1.0f, 0.0f);
         }
         else
         {
            Tileset::drawColorToTile(i, j, 1.0f, 0.0f, 0.0f);
         }
#else
         tileset->draw(i, j, tileMap[j][i]);
#endif
      }
   }
}

Map::~Map()
{
   for(int i = 0; i < width; ++i)
   {
      delete [] tileMap[i];
      delete [] passibilityMap[i];
   }

   delete [] tileMap;
   delete [] passibilityMap;

   delete pathfinder;
}
