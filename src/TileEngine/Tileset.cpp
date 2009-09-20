#include "Tileset.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include "GraphicsUtil.h"
#include "TileEngine.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_RES_LOAD | DEBUG_TILE_ENG;

Tileset::Tileset(ResourceKey name) : Resource(name)
{
}

void Tileset::load(const char* path)
{
   std::ifstream in;
   std::string imagePath;
   in.open(path);

   if(!in.is_open())
   {  T_T(std::string("Error opening file: ") + path);
   }

   in >> imagePath;
   if(!in)
   {  T_T("Tileset missing passibility matrix.");
   }

   int imgWidth, imgHeight;
   GraphicsUtil::getInstance()->loadGLTexture(imagePath.c_str(), texture, imgWidth, imgHeight);
   width = imgWidth / TileEngine::TILE_SIZE;
   height = imgHeight / TileEngine::TILE_SIZE;

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
