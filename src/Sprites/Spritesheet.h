/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef SPRITESHEET_H
#define SPRITESHEET_H

#include "Resource.h"
#include "FrameSequence.h"
#include "Rectangle.h"
#include "Size.h"
#include <map>
#include <memory>
#include <vector>
#include <string>

namespace Json
{
   class Value;
};

namespace shapes
{
   struct Point2D;
};

struct SpriteFrame;
class Animation;
class Texture;

/**
 * The Spritesheet class represents an entire spritesheet image. It holds a
 * texture with all the sprites for a given character. It also has locations for
 * each of the frames, and a listing of animations.
 * This additional frame and animation data is specified by a related .EDS file.
 *
 * @author Noam Chitayat
 */
class Spritesheet : public Resource
{
   /** The file extension used for Spritesheet image files. */
   static const std::string IMG_EXTENSION;

   /** The file extension used for Spritesheet data files. */
   static const std::string DATA_EXTENSION;

   /**
    * The default name given to an untitled frame or animation.
    * Untitled frames are a result of unfinished work on the spritesheet file;
    * maybe there were a lot of frames to name, so the content producer decided to work on the naming in parts.
    * We completely skip these untitled lines.
    */
   static const std::string UNTITLED_LINE;

   /** The spritesheet texture */
   std::unique_ptr<Texture> m_texture;

   /** Spritesheet size (in pixels) */
   shapes::Size m_size;

   /** The list of frames, which hold locations of different sprites in the sheet. */
   std::vector<shapes::Rectangle> m_frameList;

   /** The number of frames available in the spritesheet. */
   int m_numFrames;

   /** The mapping of frame names to their indices in the frameList array. */
   std::map<std::string, int> m_frameIndices;

   /** The number of animations available in the spritesheet. */
   int m_numAnimations;

   /** The mapping of animation names to their frame lists. */
   std::map<std::string, std::unique_ptr<const FrameSequence>> m_animationList;

   /**
    * Loads the spritesheet image into an OpenGL texture, and loads the
    * associated data (frames and animations). 
    *
    * @param path The path to the spritesheet image and data.
    */
   void load(const std::string& path);

   /**
    * Loads the sprite frames from the spritesheet data. 
    *
    * @param rootElement The element containing the sprite frames.
    */
   void parseFrames(Json::Value& rootElement);
   
   /**
    * Loads the sprite animations from the spritesheet data. 
    *
    * @param rootElement The element containing the sprite frames.
    */
   void parseAnimations(Json::Value& rootElement);

   public:
      /**
       * Constructor.
       *
       * @param name The name of the spritesheet Resource.
       */
      Spritesheet(ResourceKey name);

      /**
       * Draws a given frame at a specified location.
       *
       * @param point The location to draw at.
       * @param frameIndex The frame to draw.
       */
      void draw(const shapes::Point2D& point, const int frameIndex) const;

      /**
       * Get the index of a frame specified by the frame name.
       *
       * @param frameName The name of the frame to get.
       *
       * @return An index into the frame requested.
       */
      int getFrameIndex(const std::string& frameName) const;

      /**
       * Get a new animation list specified by the animation name.
       *
       * @param animationName The name of the animation to get.
       *
       * @return An animation structure that can outputs a frame index for the
       *         amount of time that has passed.
       */
      std::unique_ptr<Animation> getAnimation(const std::string& animationName) const;

      /**
       * Implementation of method in Resource class.
       *
       * @return The size of the spritesheet resource in memory.
       */
      size_t getResourceSize() const;
};

#endif
