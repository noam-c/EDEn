#ifndef __SPRITE_H_
#define __SPRITE_H_

#include <string>

class Spritesheet;
class Animation;

/**
 * A sprite is a movable object that can go through different animations or
 * static frames. Sprites are used to represent projectiles, characters,
 * anything that isn't background.
 */
class Sprite
{
   /** The spritesheet containing this sprite's frames. */
   Spritesheet* sheet;

   /** The index of the current static frame within the sheet. -1 if an animation is used instead. */
   int frameIndex;

   /** The animation structure to use to animate this sprite. NULL if a static frame is used instead. */
   Animation* animation;

   public:

      /**
       * Constructor.
       *
       * @param sheet The spritesheet to use to draw this sprite.
       */
      Sprite(Spritesheet* sheet);

      /**
       * Clears any existing frame or animation information.
       */
      void clearCurrentFrame();

      /**
       * Change the spritesheet for this sprite (changes the look of it)
       *
       * @param newSheet The new Spritesheet to use.
       */
      void setSheet(Spritesheet* newSheet);

      /**
       * Set a static frame to draw for this sprite.
       *
       * @param frameName The name of the static frame.
       */
      void setFrame(const std::string& frameName);

      /**
       * Set an animation to draw for this sprite.
       *
       * @param animationName The name of the animation.
       */
      void setAnimation(const std::string& animationName);

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
       * @param x The x-location to draw at.
       * @param y The y-location to draw at.
       */
      void draw(int x, int y) const;

      /**
       * Destructor.
       */
      ~Sprite();
};

#endif
