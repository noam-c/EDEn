/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include "TriggerZone.h"

TriggerZone::TriggerZone(const std::string& name, const geometry::Rectangle& bounds) :
   m_name(name),
   m_bounds(bounds)
{}

const geometry::Rectangle& TriggerZone::getBounds() const
{
    return m_bounds;
}

const std::string TriggerZone::getName() const
{
    return m_name;
}
