/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef TILESET_H
#define TILESET_H

#include "Resource.h"

typedef unsigned int GLuint;

/**
 * This resource holds a tileset image texture and associated data
 * including dimensions (in tiles) and default passibility of each tile.
 *
 * @author Noam Chitayat
 */
class Tileset : public Resource
{
   /** The file extension used for Spritesheet image files */
   static const std::string IMG_EXTENSION;

   /** The file extension used for Spritesheet data files */
   static const std::string DATA_EXTENSION;

   /** Width (in tiles) */
   int width;

   /** Height (in tiles) */
   int height;

   /** Passibility matrix */
   bool** passibility;

   /** The tile texture */
   GLuint texture;

   void load(const char* path);

   public:

      /**
       * Constructor.
       *
       * @param name The name of this tileset Resource.
       */
      Tileset(ResourceKey name);

      /**
       * Implementation of method in Resource class.
       *
       * @return The size of the tileset resource in memory.
       */
      size_t getSize();

      /**  
       * @return the width (in tiles) of the tileset
       */
      int getWidth();

      /**  
       * @return the height (in tiles) of the tileset
       */
      int getHeight();

      /**
       * Draws the specified tile to the coordinates specified
       *
       * @param destX The destination x-location (in tiles)
       * @param destY The destination y-location (in tiles)
       * @param tileNum The index of the tile to draw
       */
      void draw(int destX, int destY, int tileNum);

      /**
       * Draws the specified color to the coordinates specified
       *
       * @param destX The destination x-location (in tiles)
       * @param destY The destination y-location (in tiles)
       * @param r The red element of the color to draw
       * @param g The green element of the color to draw
       * @param b The blue element of the color to draw
       */
      static void drawColorToTile(int destX, int destY, float r, float g, float b);

      /**
       * @param tileNum The index of the tile to check
       *
       * @return true iff the tile at tileNum is passible by default
       */
      bool isPassible(int tileNum) const;

      /**
       * Destructor.
       */
      ~Tileset();
};

#endif
