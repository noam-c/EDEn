/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef MESSAGE_PIPE_H
#define MESSAGE_PIPE_H

#include <set>
#include <map>

#include "Listener.h"
#include "DebugUtils.h"
#include <typeinfo>
#include <typeindex>

namespace messaging
{
   class MessagePipe
   {
      class ListenerListBase {};
      
      template<typename T> class ListenerList : public ListenerListBase
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
               for(auto listener: m_listeners)
               {
                  listener->receive(message);
               }
            }
      };

      std::map<std::type_index, ListenerListBase*> listenerLists;
      
      template<typename T> ListenerList<T>* getListenerList() const
      {
         auto listenerListIter = listenerLists.find(std::type_index(typeid(T)));
         if(listenerListIter != listenerLists.end())
         {
            return static_cast<ListenerList<T>*>(listenerListIter->second);
         }
         
         return nullptr;
      }
      
      template<typename T> ListenerList<T>* getOrCreateListenerList() {
         auto listenerList = getListenerList<T>();
         
         if(listenerList == nullptr)
         {
            listenerList = new ListenerList<T>();
            listenerLists.insert(std::make_pair(std::type_index(typeid(T)), listenerList));
         }

         return listenerList;
      }
      
      public:
         template<typename T> void registerListener(Listener<T>* listener)
         {
            getOrCreateListenerList<T>()->registerListener(listener);
         }

         template<typename T> void unregisterListener(Listener<T>* listener)
         {
            getOrCreateListenerList<T>()->unregisterListener(listener);
         }

         template<typename T> void sendMessage(const T& message) const
         {
            auto listenerList = getListenerList<T>();
            if(listenerList != nullptr)
            {
               listenerList->send(message);
            }
         }
   };
};

#endif
