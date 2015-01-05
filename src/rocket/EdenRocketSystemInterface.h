/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#ifndef EDEN_ROCKET_SYSTEM_INTERFACE_H
#define EDEN_ROCKET_SYSTEM_INTERFACE_H

#include <Rocket/Core/SystemInterface.h>

/**
 * System interface for Rocket in EDEn.
 *
 * @author Noam Chitayat
 */
class EdenRocketSystemInterface : public Rocket::Core::SystemInterface
{
   public:
      /**
       * @return the number of seconds elapsed since the start of the application.
       */
      virtual float GetElapsedTime();

      /**
       * Log a debug message using the EDEn debugging system.
       *
       * @return true iff execution should continue
       */
      virtual bool LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String& message);
};

#endif
