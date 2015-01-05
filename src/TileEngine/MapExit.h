/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#ifndef MAP_EXIT_H
#define MAP_EXIT_H

#include <string>
#include "Rectangle.h"

class MapExit
{
   std::string m_nextMap;
   shapes::Rectangle m_bounds;

   public:
      MapExit(const std::string& nextMap, const shapes::Rectangle& bounds);
      const shapes::Rectangle& getBounds() const;
      const std::string getNextMap() const;
};

#endif
