/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#ifndef XMAP_H
#define XMAP_H

#include <string>
#include "Map.h"

/**
 * A map is a subset of a Region consisting of a single rectangular set of tiles
 * drawn using a given tileset. The player character walks around in a map,
 * interacting with NPCs and casting spells. The player may leave this map and
 * cross to either another map in the same Region, or the Overworld.
 *
 * @author Noam Chitayat
 */
class XMap : public Map
{
   public:

      /**
       * Constructor. Loads map data from a Region file.
       * At the end of construction, the input stream 'in' will be at the end of
       * this Map's data.
       *
       * @param in The region file stream, currently pointing at the
       *           beginning of this Map's data.
       */
      XMap(const std::string& name, const std::string& filePath);
};

#endif
