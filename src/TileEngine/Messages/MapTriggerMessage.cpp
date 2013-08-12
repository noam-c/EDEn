#include "MapTriggerMessage.h"

MapTriggerMessage::MapTriggerMessage(const TriggerZone& triggerZone, const Actor* const triggeringActor) :
   m_triggerZone(triggerZone),
   m_triggeringActor(triggeringActor)
{}
