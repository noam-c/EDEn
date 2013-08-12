/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef MESSAGE_PIPE_H
#define MESSAGE_PIPE_H

#include <set>

#include "Listener.h"
#include "DebugUtils.h"

struct ActorMoveMessage;
struct MapTriggerMessage;
struct MapExitMessage;
struct RosterUpdateMessage;

namespace messaging
{
   class MessagePipe
   {
      template<typename T> class ListenerList
      {
         std::set<Listener<T>*> m_listeners;
         
         public:
            void registerListener(Listener<T>* listener)
            {
               m_listeners.insert(listener);
            }
            
            void unregisterListener(Listener<T>* listener)
            {
               m_listeners.erase(listener);
            }

            void send(const T& message) const
            {
               typename std::set<Listener<T>*>::const_iterator iter;
               for(iter = m_listeners.begin(); iter != m_listeners.end(); ++iter)
               {
                  (*iter)->receive(message);
               }
            }
      };

      ListenerList<ActorMoveMessage> actorMoveMessages;
      ListenerList<MapTriggerMessage> mapTriggerMessages;
      ListenerList<MapExitMessage> mapExitMessages;
      ListenerList<RosterUpdateMessage> rosterUpdateMessages;
      
      public:
         template<typename T> void registerListener(Listener<T>* listener)
         {
            T_T("This listener type is unsupported.");
         }

         template<typename T> void unregisterListener(Listener<T>* listener)
         {
            T_T("This listener type is unsupported.");
         }

         template<typename T> void sendMessage(const T& message) const
         {
            T_T("This message type is unsupported.");
         }
   };

   template<> void MessagePipe::registerListener(Listener<ActorMoveMessage>* listener);
   template<> void MessagePipe::unregisterListener(Listener<ActorMoveMessage>* listener);
   template<> void MessagePipe::sendMessage(const ActorMoveMessage& message) const;

   template<> void MessagePipe::registerListener(Listener<MapTriggerMessage>* listener);
   template<> void MessagePipe::unregisterListener(Listener<MapTriggerMessage>* listener);
   template<> void MessagePipe::sendMessage(const MapTriggerMessage& message) const;

   template<> void MessagePipe::registerListener(Listener<MapExitMessage>* listener);
   template<> void MessagePipe::unregisterListener(Listener<MapExitMessage>* listener);
   template<> void MessagePipe::sendMessage(const MapExitMessage& message) const;

   template<> void MessagePipe::registerListener(Listener<RosterUpdateMessage>* listener);
   template<> void MessagePipe::unregisterListener(Listener<RosterUpdateMessage>* listener);
   template<> void MessagePipe::sendMessage(const RosterUpdateMessage& message) const;
};

#endif
