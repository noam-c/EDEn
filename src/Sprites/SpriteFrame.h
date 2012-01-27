/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef SPRITE_FRAME_H
#define SPRITE_FRAME_H

/**
 * A sprite frame is a single, non-animated (static) frame represented by
 * the coordinates of a sprite in a spritesheet. This could be the frame for
 * a character standing facing downward, taking a step upward, anything, really.
 * The coordinates in this object create a frame that represents all four edges
 * of a given sprite frame.
 *
 * @author Noam Chitayat
 */
struct SpriteFrame
{
   /** The pixel locations of the edges of this sprite in the spritesheet */
   int left, top, right, bottom;

   /**
    * Constructor.
    * \todo Is this constructor really necessary? I'd rather not create
    *       uninitialized frames.
    */
   SpriteFrame();

   /**
    * Constructor.
    *
    * @param left The left edge of the frame.
    * @param top The top edge of the frame.
    * @param right The right edge of the frame.
    * @param bottom The bottom edge of the frame.
    */
   SpriteFrame(int left, int top, int right, int bottom);
};

#endif
