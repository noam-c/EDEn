/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef EDEN_ROCKET_SYSTEM_INTERFACE_H
#define EDEN_ROCKET_SYSTEM_INTERFACE_H

#include <Rocket/Core/SystemInterface.h>

/**
 * System interface for Rocket in EDEn.
 *
 * @author Noam Chitayat
 */
class EdenRocketSystemInterface final : public Rocket::Core::SystemInterface
{
   public:
      /**
       * @return the number of seconds elapsed since the start of the application.
       */
      float GetElapsedTime() override;

      /**
       * Log a debug message using the EDEn debugging system.
       *
       * @return true iff execution should continue
       */
      bool LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String& message) override;
};

#endif
