/*
*  This file is covered by the Ruby license. See LICENSE.txt for more details.
*
*  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
*/

#ifndef LISTENER_H
#define LISTENER_H

namespace messaging
{
   template<typename T> class Listener
   {
      public:
         virtual void receive(const T& message) = 0;
         virtual ~Listener() {}
   };
};

#endif
