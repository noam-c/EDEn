/*
*  This file is covered by the Ruby license. See LICENSE.txt for more details.
*
*  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
*/

#ifndef LISTENER_H
#define LISTENER_H

namespace messaging
{
   template<typename T> class Listener
   {
      public:
         /**
          * Handle a message from the message pipe.
          *
          * @param message The message sent to the listener.
          */
         virtual void receive(const T& message) = 0;

         /**
          * Destructor.
          */
         virtual ~Listener() = default;
   };
};

#endif
