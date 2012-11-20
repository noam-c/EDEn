/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "TriggerZone.h"

TriggerZone::TriggerZone(const std::string& name, const shapes::Rectangle& bounds) :
   name(name),
   bounds(bounds)
{}

const shapes::Rectangle& TriggerZone::getBounds() const
{
    return bounds;
}

const std::string TriggerZone::getName() const
{
    return name;
}
