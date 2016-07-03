/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef ACTOR_H
#define ACTOR_H

#include <string>

#include "Size.h"
#include "Point2D.h"

namespace geometry
{
   enum class Direction;
};

class Sprite;

class Actor
{
   protected:
      /**
       * Constructor for the actor.
       *
       * @param name The name of the actor (must also be the name of its script).
       * @param location The location (in pixels) where the actor will start off.
       * @param size The size (in pixels) of the actor.
       * @param direction The starting direction of the actor.
       */
      Actor(const std::string& name, const geometry::Point2D& location, const geometry::Size& size, geometry::Direction direction);

      Actor(Actor&&);
      Actor& operator=(Actor&&);

      Actor(const Actor&) = delete;
      Actor& operator=(const Actor&) = delete;

      virtual ~Actor();

      /** The Actor's name */
      std::string m_name;
      
      /** The direction that the actor is currently facing */
      geometry::Direction m_currDirection;
   
      /** The size of the actor (in pixels) */
      geometry::Size m_size;
   
      /** The current location of the actor (in pixels) */
      geometry::Point2D m_pixelLoc;
   
      /** The Actor's associated sprite, which is drawn on screen. */
      std::unique_ptr<Sprite> m_sprite;

   public:
      /** The default animation set to use when the Actor is moving. */
      const static std::string DEFAULT_WALKING_PREFIX;
      
      /** The default frame set to use when the Actor is not moving. */
      const static std::string DEFAULT_STANDING_PREFIX;

      /**
       * Performs a logic step of this actor, updating the sprite/animation.
       *
       * @param timePassed The amount of time that has passed since the last frame.
       */
      virtual void step(long timePassed);

      /**
       * This function draws the actor in its current location with its current
       * sprite animation frame.
       */
      virtual void draw() const;

      /**
       * @return The size of this Actor (in pixels).
       */
      const geometry::Size& getSize() const;

      /**
       * @return The name of this Actor.
       */
      const std::string& getName() const;

      /**
       * This function changes the actor's spritesheet.
       *
       * @param sheetName The name of the spritesheet to get.
       */
      void setSpritesheet(const std::string& sheetName);
      
      /**
       * This function changes the actor's frame.
       *
       * @param frameName The name of the frame to use.
       */
      void setFrame(const std::string& frameName);
      
      /**
       * This function changes the actor's animation.
       *
       * @param animationName The name of the animation to use.
       *
       */
      void setAnimation(const std::string& animationName);
   
      /**
       * Change the location of the actor.
       * NOTE: This method is used for instantly changing the
       * location of the actor. To have the actor move to a new location,
       * please invoke Actor::move instead.
       *
       * @param location The new location of the actor.
       */
      virtual void setLocation(const geometry::Point2D& location);
      
      /**
       * @return The location of the actor.
       */
      const geometry::Point2D& getLocation() const;
   
      /**
       * This function changes the direction that the actor is facing.
       *
       * @param direction The new direction for the actor to face.
       */
      void setDirection(geometry::Direction direction);
      
      /**
       * @return The direction that the actor is currently facing.
       */
      geometry::Direction getDirection() const;
};

#endif
