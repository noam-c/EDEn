/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "MapExit.h"

MapExit::MapExit(const std::string& nextMap, const shapes::Rectangle& bounds) :
   nextMap(nextMap),
   bounds(bounds)
{}

const shapes::Rectangle& MapExit::getBounds() const
{
    return bounds;
}

const std::string MapExit::getNextMap() const
{
    return nextMap;
}
