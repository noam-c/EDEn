#include "Tileset.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
#include "SDL_opengl.h"
#include "DebugUtils.h"
#include <fstream>

const int Tileset::TILE_SIZE = 64;

Tileset::Tileset(const char* path) throw(Exception)
{  std::ifstream in;
   std::string imagePath;
   in.open(path);

   if(!in.is_open())
   {  T_T(std::string("Error opening file: ") + path);
   }

   in >> imagePath;
   if(!in)
   {  T_T("Tileset missing passibility matrix.");
   }

   char* p = new char[imagePath.length()];
   strcpy(p, imagePath.c_str());

   try
   {  loadGLTexture(p);
   }
   catch(Exception e)
   {  throw e;
   }

   passibility = new bool*[width];

   for(int i = 0; i < width; ++i)
   {  passibility[i] = new bool[height];
      for(int j = 0; j < height; ++j)
      {  if(in)
         {  in >> passibility[i][j];
         }
         else
         {  T_T("Tileset has incomplete passibility matrix.");
         }         
      }
   }

}
   
bool Tileset::loadGLTexture(char* path)
{   // Create storage space for the texture
    SDL_Surface *tilesetImage; 

    // Create the texture
    glGenTextures(1, &texture);

    // Load the bitmap
    if ((tilesetImage = SDL_LoadBMP(path)))
    {  // Bitmap load successful
       // Load in texture
	    // Typical texture generation using bitmap data
	    glBindTexture(GL_TEXTURE_2D, texture);

	    // Linear filtering
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	    // Generate the texture
	    glTexImage2D(GL_TEXTURE_2D, 0, 3, tilesetImage->w,
			  tilesetImage->h, 0, GL_BGR,
			  GL_UNSIGNED_BYTE, tilesetImage->pixels);

       width = tilesetImage->w / TILE_SIZE;
       height = tilesetImage->h / TILE_SIZE;

       SDL_FreeSurface(tilesetImage);

       return true;
    }
    
    // Problem loading the bitmap; throw an exception
    throw (EXCEPTION_INFO, std::string("Unable to load tileset: ") + path);
}

void Tileset::draw(int destX, int destY, int tileNum)
{  int tilesetX = tileNum % width;
   int tilesetY = tileNum / width;

   float tileRight = (tilesetX + 1) * TILE_SIZE - 1;
   float tileBottom = (tilesetY + 1) * TILE_SIZE - 1;

   float top = float(tilesetY) / height;
   float bottom = tileBottom / (height*TILE_SIZE - 1);
   float left = float(tilesetX) / width;
   float right = tileRight / (width*TILE_SIZE - 1);

   glBegin(GL_QUADS);
      glTexCoord2f(left, top); glVertex3f(float(destX), float(destY), 0.0f);
      glTexCoord2f(right, top); glVertex3f(float(destX + 1), float(destY), 0.0f);
      glTexCoord2f(right, bottom); glVertex3f(float(destX + 1), float(destY + 1), 0.0f);
      glTexCoord2f(left, bottom); glVertex3f(float(destX), float(destY + 1), 0.0f);
   glEnd();
}

size_t Tileset::getSize()
{  return sizeof(this);
}
   
Tileset::~Tileset()
{  for(int i = 0; i < width; ++i)
   {  delete [] passibility[i];
   }

   delete [] passibility;

   glDeleteTextures(1, &texture);
}
