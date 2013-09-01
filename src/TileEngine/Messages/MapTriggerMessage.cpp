#include "MapTriggerMessage.h"

MapTriggerMessage::MapTriggerMessage(const TriggerZone& triggerZone, const Actor* const triggeringActor) :
   triggerZone(triggerZone),
   triggeringActor(triggeringActor)
{}
