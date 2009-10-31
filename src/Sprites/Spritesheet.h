#ifndef __SPRITESHEET_H_
#define __SPRITESHEET_H_

#include "Resource.h"
#include <map>
#include <string>

struct Frame;
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
 * \todo Document.
 */
class Spritesheet : public Resource
{
   static const std::string IMG_EXTENSION;
   static const std::string DATA_EXTENSION;

   /** The spritesheet texture */
   GLuint texture;

   /** Width (in pixels) */
   int width;

   /** Height (in pixels) */
   int height;

   int numFrames;
   std::map<std::string, int> frameIndices;
   Frame** frameList;

   int numAnimations;
   std::map<std::string, FrameNode*> animationList;

   void load(const char* path);

   public:
      Spritesheet(ResourceKey name);
      void draw(int x, int y, int frameIndex);
      int getFrameIndex(std::string frameName);
      Animation* getAnimation(std::string animationName);
      size_t getSize();
};

#endif
