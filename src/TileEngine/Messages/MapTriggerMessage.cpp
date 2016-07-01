/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "MapTriggerMessage.h"

MapTriggerMessage::MapTriggerMessage(const TriggerZone& triggerZone, const GridActor* const triggeringActor) :
   triggerZone(triggerZone),
   triggeringActor(triggeringActor)
{}
