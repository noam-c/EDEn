/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef TRIGGER_DETECTOR_H
#define TRIGGER_DETECTOR_H

#include <string>
#include "Listener.h"
#include "Rectangle.h"

struct ActorMoveMessage;

namespace messaging
{
	class MessagePipe;
};

class TriggerDetector : public messaging::Listener<ActorMoveMessage>
{
   messaging::MessagePipe& messagePipe;
   const TriggerZone& triggerZone;

   public:
      TriggerDetector(messaging::MessagePipe& messagePipe, const TriggerZone& triggerZone);
      void receive(const ActorMoveMessage& message);
};

#endif
