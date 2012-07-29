#ifndef EDEN_ROCKET_SYSTEM_H
#define EDEN_ROCKET_SYSTEM_H

#include <Rocket/Core/SystemInterface.h>

class EdenRocketSystem : public Rocket::Core::SystemInterface
{
   public:
      /**
       * @return the number of seconds elapsed since the start of the application.
       */
      virtual float GetElapsedTime();
};

#endif
