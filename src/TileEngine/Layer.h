/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef LAYER_H
#define LAYER_H

#include <functional>
#include <memory>
#include <string>

#include "Grid.h"

namespace geometry
{
   struct Rectangle;
};

class Tileset;
class TiXmlElement;

class Layer
{
   /** The name of the tileset in use by this layer */
   std::string m_tilesetName;

   /** Tileset in use by this map */
   std::shared_ptr<Tileset> m_tileset;

   /** The tiles of the map (as numbers referring to points in the Tileset) */
   Grid<int> m_tileMap;

   /** The bounds (in tiles) of the map containing this layer. */
   const geometry::Rectangle& m_bounds;

   /** The height offset (in tiles) of this layer. */
   int m_heightOffset = 0;

   public:
      /**
       * Constructor.
       *
       * @param layerData The map data for this layer.
       * @param bounds The bounds of the map.
       */
      Layer(const TiXmlElement* layerData, const geometry::Rectangle& bounds);

      /**
       * Perform the given function for each collision rectangle in this layer.
       *
       * @param func The iterator function to perform on each collision rectangle.
       */
      void forEachCollisionRect(std::function<void(const geometry::Rectangle&)>&& func) const;

      /**
       * Draws a row of the layer to screen.
       *
       * @param row The row to draw from the layer.
       * @param isForeground Set true iff this layer is being drawn in the foreground.
       */
      void draw(int row, bool isForeground = false) const;
};

#endif
