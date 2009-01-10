#include "Map.h"
#include "GraphicsUtil.h"
#include "ResourceLoader.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_RES_LOAD;

Map::Map(std::ifstream& in)
{  std::getline(in, mapName, MAP_DELIM);
   DEBUG2("Loading map: ", mapName);
   std::getline(in, tilesetName, MAP_DELIM);

   in >> width;
   in.get();
   in >> height;

   tileset = ResourceLoader::getTileset(tilesetName);

   tileMap = new int*[width];

   for(int i = 0; i < width; ++i)
   {  tileMap[i] = new int[height];
      for(int j = 0; j < height; ++j)
      {  if(in)
         {  in >> tileMap[i][j];
         }
         else
         {  T_T("Tile map incomplete.");
         }
      }
      in.ignore(width<<1, '\n');
   }
   DEBUG("Map loaded.");
}

std::string Map::getName()
{  return mapName;
}

void Map::draw()
{  GraphicsUtil::getInstance()->clearBuffer();

   for(int i = 0, k = width - 1; i < width; ++i, --k)
   {  for(int j = 0, l = height - 1; j < height; ++j, --l)
      {  tileset->draw(i, j, tileMap[l][i]);
      }
   }
}

Map::~Map()
{  for(int i = 0; i < width; ++i)
   {  delete [] tileMap[i];
   }

   delete [] tileMap;
}
