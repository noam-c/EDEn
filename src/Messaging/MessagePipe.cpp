/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "MessagePipe.h"
#include "Listener.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_MESSAGING;

namespace messaging
{
   template<> void MessagePipe::registerListener(Listener<ActorMoveMessage>* listener)
   {
      actorMoveMessages.registerListener(listener);
   }

   template<> void MessagePipe::unregisterListener(Listener<ActorMoveMessage>* listener)
   {
      actorMoveMessages.unregisterListener(listener);
   }

   template<> void MessagePipe::sendMessage(const ActorMoveMessage& message)
   {
      actorMoveMessages.send(message);
   }

   template<> void MessagePipe::registerListener(Listener<MapTriggerMessage>* listener)
   {
      mapTriggerMessages.registerListener(listener);
   }

   template<> void MessagePipe::unregisterListener(Listener<MapTriggerMessage>* listener)
   {
      mapTriggerMessages.unregisterListener(listener);
   }

   template<> void MessagePipe::sendMessage(const MapTriggerMessage& message)
   {
      DEBUG("Pipe received map trigger message.");
      mapTriggerMessages.send(message);
   }

   template<> void MessagePipe::registerListener(Listener<MapExitMessage>* listener)
   {
      mapExitMessages.registerListener(listener);
   }

   template<> void MessagePipe::unregisterListener(Listener<MapExitMessage>* listener)
   {
      mapExitMessages.unregisterListener(listener);
   }

   template<> void MessagePipe::sendMessage(const MapExitMessage& message)
   {
      DEBUG("Pipe received map exit message.");
      mapExitMessages.send(message);
   }
};
