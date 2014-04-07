/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef MESSAGE_PIPE_H
#define MESSAGE_PIPE_H

#include <set>
#include <map>
#include <memory>
#include <typeindex>
#include <typeinfo>

#include "Listener.h"
#include "DebugUtils.h"

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
               m_listeners.insert(std::move(listener));
            }
            
            void unregisterListener(Listener<T>* listener)
            {
               m_listeners.erase(std::move(listener));
            }

            void send(const T& message) const
            {
               typename std::set<Listener<T>*>::const_iterator iter;
               for(auto& listener : m_listeners)
               {
                  listener->receive(message);
               }
            }
      };

      std::map<std::type_index, std::unique_ptr<ListenerListBase>> listenerLists;
      
      template<typename T> ListenerList<T>* getListenerList() const
      {
         auto listenerListIter = listenerLists.find(std::type_index(typeid(T)));
         if(listenerListIter != listenerLists.end())
         {
            return static_cast<ListenerList<T>*>(listenerListIter->second.get());
         }
         
         return nullptr;
      }
      
      template<typename T> ListenerList<T>* getOrCreateListenerList() {
         auto listenerList = getListenerList<T>();
         
         if(listenerList)
         {
            return listenerList;
         }

         auto newListenerList = new ListenerList<T>();
         listenerLists.insert(std::make_pair(std::type_index(typeid(T)), std::unique_ptr<ListenerList<T>>(newListenerList)));
         return newListenerList;
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
            if(listenerList)
            {
               listenerList->send(message);
            }
         }
   };
};

#endif
