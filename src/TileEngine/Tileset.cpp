#include "Tileset.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include "GraphicsUtil.h"
#include "TileEngine.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_RES_LOAD | DEBUG_TILE_ENG;

const std::string Tileset::IMG_EXTENSION = ".png";
const std::string Tileset::DATA_EXTENSION = ".edt";

Tileset::Tileset(ResourceKey name) : Resource(name)
{
}

void Tileset::load(const char* path)
{
   std::string imagePath(path);
   imagePath += IMG_EXTENSION;

   DEBUG("Loading tileset image \"%s\"...", imagePath.c_str());

   int imgWidth, imgHeight;
   GraphicsUtil::getInstance()->loadGLTexture(imagePath.c_str(), texture, imgWidth, imgHeight);

   width = imgWidth / TileEngine::TILE_SIZE;
   height = imgHeight / TileEngine::TILE_SIZE;

   // A data file is needed to hold the default passibility matrix for a Tileset
   // Since maps will rarely change the passibility of tiles, it doesn't make
   // sense to hold passibility within each map's data; a map's Lua script
   // should be able to cheat default passibility if necessary.
   std::string dataPath(path);
   dataPath += DATA_EXTENSION;
   DEBUG("Loading tileset data \"%s\"...", dataPath.c_str());

   std::ifstream in(dataPath.c_str());
   in.open(path);
   if(!in.is_open())
   {  T_T(std::string("Error opening file: ") + path);
   }

   passibility = new bool*[width];

   for(int i = 0; i < width; ++i)
   {
      passibility[i] = new bool[height];
      for(int j = 0; j < height; ++j)
      {
         if(in)
         {
            in >> passibility[i][j];
         }
         else
         {
            T_T("Tileset has incomplete passibility matrix.");
         }
      }
   }
}
   
void Tileset::draw(int destX, int destY, int tileNum)
{
   int tilesetX = tileNum % width;
   int tilesetY = tileNum / width;

   float destLeft = float(destX * TileEngine::TILE_SIZE);
   float destRight = float((destX + 1) * TileEngine::TILE_SIZE);
   float destTop = float(destY * TileEngine::TILE_SIZE);
   float destBottom = float((destY + 1) * TileEngine::TILE_SIZE);

   float tileRight = (tilesetX + 1) * TileEngine::TILE_SIZE - 1;
   float tileBottom = (tilesetY + 1) * TileEngine::TILE_SIZE - 1;

   float top = float(tilesetY) / height;
   float bottom = tileBottom / (height * TileEngine::TILE_SIZE - 1);
   float left = float(tilesetX) / width;
   float right = tileRight / (width * TileEngine::TILE_SIZE - 1);

   glBindTexture(GL_TEXTURE_2D, texture);

   glBegin(GL_QUADS);
      glTexCoord2f(left, top); glVertex3f(destLeft, destTop, 0.0f);
      glTexCoord2f(right, top); glVertex3f(destRight, destTop, 0.0f);
      glTexCoord2f(right, bottom); glVertex3f(destRight, destBottom, 0.0f);
      glTexCoord2f(left, bottom); glVertex3f(destLeft, destBottom, 0.0f);
   glEnd();
}

size_t Tileset::getSize()
{
   return sizeof(this);
}
   
Tileset::~Tileset()
{
   for(int i = 0; i < width; ++i)
   {
      delete [] passibility[i];
   }

   delete [] passibility;

   glDeleteTextures(1, &texture);
}
