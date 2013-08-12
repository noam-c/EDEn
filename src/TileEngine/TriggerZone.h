/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef TRIGGER_ZONE_H
#define TRIGGER_ZONE_H

#include <string>
#include "Rectangle.h"

class TriggerZone
{
   const std::string m_name;
   shapes::Rectangle m_bounds;

   public:
      TriggerZone(const std::string& name, const shapes::Rectangle& bounds);
      const shapes::Rectangle& getBounds() const;
      const std::string getName() const;
};

#endif
