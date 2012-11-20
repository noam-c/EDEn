#include "ActorMoveMessage.h"

ActorMoveMessage::ActorMoveMessage(const shapes::Point2D& oldLocation, const shapes::Point2D& newLocation, const Actor* const movingActor) :
   oldLocation(oldLocation),
   newLocation(newLocation),
   movingActor(movingActor)
{}
