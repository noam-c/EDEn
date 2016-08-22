/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef TRIGGER_ZONE_H
#define TRIGGER_ZONE_H

#include <string>
#include "Rectangle.h"

class TriggerZone final
{
   std::string m_name;
   geometry::Rectangle m_bounds;

   public:
      TriggerZone(const std::string& name, const geometry::Rectangle& bounds);
      const geometry::Rectangle& getBounds() const;
      const std::string getName() const;
};

#endif
