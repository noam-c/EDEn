/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#ifndef DIRECTION_H
#define DIRECTION_H

/**
 * Represents one of the eight directions of movement used by NPCs and the player in the game.
 */
enum class Direction
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
