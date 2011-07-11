#ifndef MOVEMENT_DIRECTION_H_
#define MOVEMENT_DIRECTION_H_

/**
 * Represents one of the eight directions of movement used by NPCs and the player in the game.
 */
enum MovementDirection
{
   NONE,
   UP,
   DOWN,
   LEFT,
   RIGHT,
   UP_LEFT,
   UP_RIGHT,
   DOWN_LEFT,
   DOWN_RIGHT,
   NUM_DIRECTIONS
};

#endif