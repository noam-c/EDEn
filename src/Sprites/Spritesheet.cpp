#include "Spritesheet.h"
#include "GLInclude.h"
#include "GraphicsUtil.h"
#include "LinkedListNode.h"
#include "SpriteFrame.h"
#include "Animation.h"
#include <queue>
#include <fstream>
#include <sstream>
#include "DebugUtils.h"

const int debugFlag = DEBUG_RES_LOAD | DEBUG_SPRITE;

const std::string Spritesheet::IMG_EXTENSION = ".png";
const std::string Spritesheet::DATA_EXTENSION = ".eds";

/**
 * NOTE: If this value is changed, it MUST be changed in the Ruby Spritesheet Editor tool
 * and in any .eds files that contain the old value.
 * Otherwise, the spritesheet parsing will fail on files with two or more untitled lines.
 */
const std::string Spritesheet::UNTITLED_LINE = "untitled";

Spritesheet::Spritesheet(ResourceKey name)
                                     : Resource(name), frameList(NULL), numFrames(0)
{
}

void Spritesheet::load(const char* path)
{
   // Load the image itself into a texture using GraphicsUtil
   std::string imgPath(path);
   imgPath += IMG_EXTENSION;

   DEBUG("Loading spritesheet image \"%s\"...", imgPath.c_str());
   GraphicsUtil::getInstance()->loadGLTexture(imgPath.c_str(), texture, width, height);

   // Load in the spritesheet data file, which tells the engine where
   // each frame is in the image
   std::string dataPath(path);
   dataPath += DATA_EXTENSION;
   DEBUG("Loading spritesheet data \"%s\"...", dataPath.c_str());

   std::ifstream in(dataPath.c_str());
   if(!in.is_open())
   {
      T_T(std::string("Error opening file: ") + dataPath);
   }

   std::string line;
   std::queue<SpriteFrame> frameQueue;

   for(;;)
   {
      // Get the next line
      getline(in, line);

      if(in.eof())
      {
         // If the file is done, we are done
         break;
      }

      if(!in)
      {
         // If there is some other error, throw an exception
         T_T(std::string("Error reading from file: ") + path);
      }

      // Send the loaded line into a string stream for parsing
      std::stringstream lineStream(line, std::stringstream::in | std::stringstream::out);

      // Get the type of the line's content (static frame, or animation?)
      std::string typeName;
      lineStream >> typeName;

      if(typeName == "frame")
      {
         // Get the name of the frame
         std::string frameName;
         lineStream >> frameName;

         if(frameName == UNTITLED_LINE)
         {
            // Skip untitled frames.
            continue;
         }

         // Make sure this frame name has not already been used in this file
         if(frameIndices.find(frameName) != frameIndices.end())
         {
            DEBUG("Duplicated name %s in spritesheet %s", frameName.c_str(), path);
            T_T("Parse error reading spritesheet.");
         }

         // Get the four coordinates of the frame
         int coords[4];
         for(int i = 0; i < 4; ++i)
         {
            lineStream >> coords[i];
            if(!lineStream)
            {
               DEBUG("Error reading frame %s in spritesheet %s", frameName.c_str(), path);
               T_T("Parse error reading spritesheet.");
            }
         }

         // Create a new frame with the read coordinates
         SpriteFrame f(coords[0], coords[1], coords[2], coords[3]);
         DEBUG("Frame %s loaded in with coordinates %d, %d, %d, %d",
                         frameName.c_str(), coords[0], coords[1], coords[2], coords[3]);

         // Bind the frame name to the next available frame index
         frameIndices[frameName] = frameQueue.size();

         // Push the frame into a queue for later processing
         frameQueue.push(f);
      }
      else if(typeName == "anim")
      {
         // Get the name of the animation
         std::string animationName;
         lineStream >> animationName;

         if(animationName == UNTITLED_LINE)
         {
            // Skip untitled animations.
            continue;
         }

         // Make sure this animation name has not already been used in this file
         if(animationList.find(animationName) != animationList.end())
         {
            DEBUG("Duplicated animation name %s in spritesheet %s", animationName.c_str(), path);
            T_T("Parse error reading spritesheet.");
         }

         // Get the frames of the animation
         std::string frameName;
         FrameNode* listTail = NULL;
         for(;;)
         {
            lineStream >> frameName;

            int frameIndex = frameIndices[frameName];
            if(frameIndex < 0)
            {
               DEBUG("Found invalid frame name '%s' in animation %s", frameName.c_str(), animationName.c_str());
               T_T("Parse error reading spritesheet.");
            }

            DEBUG("Animation %s: Adding node with index %d", animationName.c_str(), frameIndex);

            if(listTail == NULL)
            {
               listTail = new FrameNode(frameIndex, NULL);
               listTail->next = listTail;
            }
            else
            {
               FrameNode* nextNode = new FrameNode(frameIndex, listTail->next);
               listTail->next = nextNode;
               listTail = nextNode;
            }
            if(lineStream.eof())
            {
               // If the line is done, there are no animations left to read
               if(listTail == NULL)
               {
                  DEBUG("Empty animation %s in spritesheet %s", animationName.c_str(), path);
                  T_T("Parse error reading spritesheet.");
               }
               break;
            }
         }

         //DEBUG("Frame %s loaded in with coordinates %d, %d, %d, %d",
         //                frameName.c_str(), coords[0], coords[1], coords[2], coords[3]);

         // Bind the animation name to the next available animation index
         animationList[animationName] = listTail->next;
      }

      // Flush any remaining line data in the stream
      /** \todo Should we be doing this? Or failing on extraneous text? */
      lineStream.flush();
   }

   // Create the new array of frames
   numFrames = frameQueue.size();
   
   if(numFrames == 0)
   {
      DEBUG("No frames found in spritesheet %s", path);
      T_T("Empty (invalid) spritesheet constructed.");
   }

   frameList = new SpriteFrame[numFrames];

   // Fill the new array with the accumulated frames
   int frameNum = 0;
   while(!frameQueue.empty())
   {
      frameList[frameNum++] = frameQueue.front(); frameQueue.pop();
   }

   DEBUG("Spritesheet constructed!");
}

int Spritesheet::getFrameIndex(const std::string& frameName) const
{
   std::map<std::string, int>::const_iterator frameIndex = frameIndices.find(frameName);
   if(frameIndex != frameIndices.end() && frameIndex->second < numFrames)
   {
      return frameIndex->second;
   }

   return -1;
}

Animation* Spritesheet::getAnimation(const std::string& animationName) const
{
   std::map<std::string, FrameNode*>::const_iterator animFrames = animationList.find(animationName);
   if(animFrames != animationList.end())
   {
      return new Animation(animFrames->second);
   }

   return NULL;
}


void Spritesheet::draw(const int x, const int y, const int frameIndex) const
{
   if(frameList == NULL)
   {
      // Don't draw if the spritesheet was not initialized
      // (i.e. there was a failure constructing this spritesheet)
      return;
   }

   if(frameIndex < 0 || frameIndex > numFrames)
   {
      DEBUG("Spritesheet frame index %d out of bounds!", frameIndex);
      return;
   }

   SpriteFrame f = frameList[frameIndex];

   /**
    * \todo We can do all these calculations when the frames are initialized
    *       in order to optimize the drawing code a bit more!
    */
   int frameHeight = f.bottom - f.top;
   int frameWidth = f.right - f.left;

   float frameTop = f.top / float(height);
   float frameBottom = f.bottom / float(height);
   float frameLeft = f.left / float(width);
   float frameRight = f.right / float(width);
   
   float destLeft = float(x);
   float destBottom = float(y);
   float destRight = destLeft + frameWidth;
   float destTop = destBottom - frameHeight;

   // NOTE: Alpha testing doesn't do transparency; it either draws a pixel or it doesn't
   // If we want fades or something like that, we would need to use alpha blending

   /** \todo Alpha testing is currently bugged. Investigate and fix. */

   // Retain old alpha test state, function and threshold
   //int oldAlphaFunction;
   //float oldAlphaThreshold;
   //glGetIntegerv(GL_ALPHA_TEST_FUNC, &oldAlphaFunction);
   //glGetFloatv(GL_ALPHA_TEST_REF, &oldAlphaThreshold);

   //bool alphaTestEnabled = glIsEnabled(GL_ALPHA_TEST);

   // Enable alpha testing
   //glEnable(GL_ALPHA_TEST);

   // Set the alpha blending evaluation function
   //glAlphaFunc(GL_GREATER, 0.1);

   glBindTexture(GL_TEXTURE_2D, texture);

   glBegin(GL_QUADS);
      glTexCoord2f(frameLeft, frameTop); glVertex3f(destLeft, destTop, 0.0f);
      glTexCoord2f(frameRight, frameTop); glVertex3f(destRight, destTop, 0.0f);
      glTexCoord2f(frameRight, frameBottom); glVertex3f(destRight, destBottom, 0.0f);
      glTexCoord2f(frameLeft, frameBottom); glVertex3f(destLeft, destBottom, 0.0f);
   glEnd();

   // We're done with alpha testing, return to default state
   //glAlphaFunc(oldAlphaFunction, oldAlphaThreshold);
   //if(!alphaTestEnabled) glDisable(GL_ALPHA_TEST);
}

size_t Spritesheet::getSize()
{
   return sizeof(Spritesheet);
}

Spritesheet::~Spritesheet()
{
   // Delete the list of animations
   std::map<std::string, FrameNode*>::iterator iter;
   for(iter = animationList.begin(); iter != animationList.end(); ++iter)
   {
      // To delete a circularly linked list, we need to first make it a linear
      // list by breaking one of the links, and then deleting from the beginning
      // of the chain to make sure every node is deleted.
      FrameNode* firstNode = iter->second;
      FrameNode* secondNode = firstNode->next;
      firstNode->next = NULL;

      delete secondNode;
   }

   // Delete all the static frames
   delete [] frameList;
}
