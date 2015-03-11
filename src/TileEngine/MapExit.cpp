/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include "MapExit.h"

MapExit::MapExit(const std::string& nextMap, const geometry::Rectangle& bounds) :
   m_nextMap(nextMap),
   m_bounds(bounds)
{}

const geometry::Rectangle& MapExit::getBounds() const
{
    return m_bounds;
}

const std::string MapExit::getNextMap() const
{
    return m_nextMap;
}
