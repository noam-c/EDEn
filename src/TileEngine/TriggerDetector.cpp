/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "TriggerDetector.h"
#include "MessagePipe.h"
#include "ActorMoveMessage.h"

TriggerDetector::TriggerDetector(messaging::MessagePipe& messagePipe, const TriggerZone& triggerZone)
	: messagePipe(messagePipe), triggerZone(triggerZone)
{
	messagePipe.registerListener(this);
}

void TriggerDetector::receive(const ActorMoveMessage& message)
{
	if(!triggerZone.getBounds().contains(message.oldLocation)
			&& triggerZone.getBounds().contains(message.newLocation))
	{
		messagePipe.sendMessage(MapTriggerMessage(triggerZone, message.movingActor));
	}
}

TriggerDetector::~TriggerDetector()
{
	messagePipe.unregisterListener(this);
}
