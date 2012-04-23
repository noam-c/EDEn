/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "MessagePipe.h"
#include "Listener.h"

#include "PlayerMoveMessage.h"

#include "DebugUtils.h"

namespace messaging
{
   template<typename T> void MessagePipe::registerListener(Listener<T>* listener)
   {
      T_T("This listener type is unsupported.");
   }

   template<typename T> void MessagePipe::unregisterListener(Listener<T>* listener)
   {
      T_T("This listener type is unsupported.");
   }

   template<typename T> void MessagePipe::sendMessage(const T& message)
   {
      T_T("This message type is unsupported.");
   }

   template<> void MessagePipe::registerListener<PlayerMoveMessage>(Listener<PlayerMoveMessage>* listener)
   {
      playerMoveMessages.registerListener(listener);
   }

   template<> void MessagePipe::unregisterListener<PlayerMoveMessage>(Listener<PlayerMoveMessage>* listener)
   {
      playerMoveMessages.unregisterListener(listener);
   }

   template<> void MessagePipe::sendMessage<PlayerMoveMessage>(const PlayerMoveMessage& message)
   {
      playerMoveMessages.send(message);
   }
};