/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef MESSAGE_PIPE_H
#define MESSAGE_PIPE_H

#include <functional>
#include <algorithm>
#include <map>
#include <set>

namespace messaging
{
   template<typename T> class Listener;

   struct PlayerMoveMessage;

   class MessagePipe
   {
      template<typename T> class ListenerList
      {
         std::set<Listener<T>*> listeners;
         
         public:
            void registerListener(Listener<T>* listener)
            {
               listeners.insert(listener);
            }
            
            void unregisterListener(Listener<T>* listener)
            {
               listeners.erase(listener);
            }

            void send(const T& message)
            {
               typename std::set<Listener<T>*>::iterator iter;
               for(iter = listeners.begin(); iter != listeners.end(); ++iter)
               {
                  (*iter)->receive(message);
               }
            }
      };

      ListenerList<PlayerMoveMessage> playerMoveMessages;
      
      template<typename T> void registerListener(Listener<T>* listener);
      template<typename T> void unregisterListener(Listener<T>* listener);
      template<typename T> void sendMessage(const T& message);
   };
};
#endif