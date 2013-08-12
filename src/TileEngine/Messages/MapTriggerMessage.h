/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef MAP_TRIGGER_MESSAGE_H
#define MAP_TRIGGER_MESSAGE_H

class Actor;
class TriggerZone;

struct MapTriggerMessage
{
   const TriggerZone& m_triggerZone;
   const Actor* const m_triggeringActor;
   
   MapTriggerMessage(const TriggerZone& triggerZone, const Actor* const movingActor);
};

#endif
