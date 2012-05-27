/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef LAYER_H
#define LAYER_H

#include <string>

namespace shapes
{
   struct Rectangle;
};

class Tileset;
class TiXmlElement;

class Layer
{
   /** The name of the tileset in use by this layer */
   std::string tilesetName;

   /** Tileset in use by this map */
   Tileset* tileset;

   /** The tiles of the map (as numbers referring to points in the Tileset) */
   int** tileMap;

   /** The bounds (in tiles) of the map containing this layer. */
   const shapes::Rectangle& bounds;

   /** The height offset (in tiles) of this layer. */
   int heightOffset;

   public:
      Layer(const TiXmlElement* layerData, const shapes::Rectangle& bounds);
      void draw(int row, bool isForeground = false) const;
      ~Layer();
};

#endif
