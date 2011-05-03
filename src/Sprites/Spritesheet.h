#ifndef __SPRITESHEET_H_
#define __SPRITESHEET_H_

#include "Resource.h"
#include <map>
#include <string>

struct SpriteFrame;
class Animation;
template<class C> struct LinkedListNode;

typedef unsigned int GLuint;

/**
 * Holds integers in a linked list structure to signify frames of an animation
 *
 * @author Noam Chitayat
 */
typedef LinkedListNode<int> FrameNode;

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
   GLuint texture;

   /** Width (in pixels) */
   int width;

   /** Height (in pixels) */
   int height;

   /** The number of frames available in the spritesheet. */
   int numFrames;

   /** The array of frames, which hold locations of different sprites in the sheet. */
   SpriteFrame* frameList;

   /** The mapping of frame names to their indices in the frameList array. */
   std::map<std::string, int> frameIndices;

   /** The number of animations available in the spritesheet. */
   int numAnimations;

   /** The mapping of animation names to their frame lists. */
   std::map<std::string, FrameNode*> animationList;

   /**
    * Loads the spritesheet image into an OpenGL texture, and loads the
    * associated data (frames and animations). 
    *
    * @param path The path to the spritesheet image and data.
    */
   void load(const char* path);

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
       * @param x The x-location to draw at.
       * @param y The y-location to draw at.
       * @param frameIndex The frame to draw.
       */
      void draw(const int x, const int y, const int frameIndex) const;

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
      Animation* getAnimation(const std::string& animationName) const;

      /**
       * Implementation of method in Resource class.
       *
       * @return The size of the spritesheet resource in memory.
       */
      size_t getSize();

      /**
       * Destructor.
       */
      ~Spritesheet();
};

#endif
