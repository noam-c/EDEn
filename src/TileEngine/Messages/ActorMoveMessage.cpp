#include "ActorMoveMessage.h"

ActorMoveMessage::ActorMoveMessage(const geometry::Point2D& oldLocation, const geometry::Point2D& newLocation, const Actor* const movingActor) :
   oldLocation(oldLocation),
   newLocation(newLocation),
   movingActor(movingActor)
{}
