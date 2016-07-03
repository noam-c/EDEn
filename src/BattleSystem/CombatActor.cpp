/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "CombatActor.h"

CombatActor::CombatActor(const std::string& name, const geometry::Point2D& location, const geometry::Size& size, geometry::Direction direction) :
   Actor(name, location, size, direction)
{
}

CombatActor::CombatActor(CombatActor&&) = default;
CombatActor& CombatActor::operator=(CombatActor&&) = default;

CombatActor::~CombatActor() = default;
