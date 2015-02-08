/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#ifndef SPRITE_H
#define SPRITE_H

#include <memory>
#include <string>

namespace shapes
{
   struct Point2D;
};

class Spritesheet;
class Animation;
enum class MovementDirection;

/**
 * A sprite is a movable object that can go through different animations or
 * static frames. Sprites are used to represent projectiles, characters,
 * anything that isn't background.
 */
class Sprite
{
   /** The spritesheet containing this sprite's frames. */
   std::shared_ptr<Spritesheet> m_sheet;

   /** The index of the current static frame within the sheet. -1 if an animation is used instead. */
   int m_frameIndex;

   /** The animation structure to use to animate this sprite. nullptr if a static frame is used instead. */
   std::unique_ptr<Animation> m_animation;

   /** The name of the current frame/animation being used. */
   std::string m_currName;

   /** The direction that the current frame/animation is facing. */
   MovementDirection m_currDirection;

   /**
    * @param direction A direction to convert to a string.
    *
    * @return The string to append to a frame or animation for a given direction.
    */
   std::string toDirectionString(MovementDirection direction);

   public:

      /**
       * Constructor.
       *
       * @param sheet The spritesheet to use to draw this sprite.
       */
      Sprite(const std::shared_ptr<Spritesheet>& sheet);

      /**
       * Destructor.
       */
      ~Sprite();

      /**
       * Clears any existing frame or animation information.
       */
      void clearCurrentFrame();

      /**
       * Change the spritesheet for this sprite (changes the look of it)
       *
       * @param sheet The new Spritesheet to use.
       */
      void setSheet(const std::shared_ptr<Spritesheet>& sheet);

      /**
       * Set a static frame to draw for this sprite.
       *
       * @param frameName The name of the static frame.
       * @param direction The direction that the new sprite should face.
       */
      void setFrame(const std::string& frameName, MovementDirection direction);

      /**
       * Set an animation to draw for this sprite.
       *
       * @param animationName The name of the animation.
       * @param direction The direction that the new sprite should face.
       */
      void setAnimation(const std::string& animationName, MovementDirection direction);

      /**
       * A logic step for the sprite. Currently just advances the animation if
       * there is one.
       *
       * @param timePassed The amount of time to advance forward.
       */
      void step(long timePassed);

      /**
       * Draws the sprite at the specified location.
       *
       * @param point The location to draw at.
       */
      void draw(const shapes::Point2D& point) const;
};

#endif
