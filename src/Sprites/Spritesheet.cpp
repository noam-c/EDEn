/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "Spritesheet.h"
#include "SDL_opengl.h"
#include "GraphicsUtil.h"
#include "Texture.h"
#include "Rectangle.h"
#include "Point2D.h"
#include "Animation.h"
#include <queue>
#include <fstream>
#include <sstream>
#include "json.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_RES_LOAD | DEBUG_SPRITE;

const std::string Spritesheet::IMG_EXTENSION = ".png";
const std::string Spritesheet::DATA_EXTENSION = ".eds";

/**
 * NOTE: If this value is changed, it MUST be changed in the TagSprite Editor tool
 * and in any .eds files that contain the old value.
 * Otherwise, the spritesheet parsing will fail on files with two or more untitled lines.
 */
const std::string Spritesheet::UNTITLED_LINE = "untitled";

Spritesheet::Spritesheet(ResourceKey name) : Resource(name), frameList(NULL), numFrames(0), texture(NULL)
{
}

void Spritesheet::load(const std::string& path)
{
   // Load the image itself into a texture using GraphicsUtil
   std::string imgPath(path);
   imgPath += IMG_EXTENSION;

   DEBUG("Loading spritesheet image \"%s\"...", imgPath.c_str());
   texture = new Texture(imgPath);
   size = texture->getSize();

   // Load in the spritesheet data file, which tells the engine where
   // each frame is in the image
   std::string dataPath(path);
   dataPath += DATA_EXTENSION;
   DEBUG("Loading spritesheet data \"%s\"...", dataPath.c_str());

   std::ifstream input(dataPath.c_str());
   if(!input.is_open())
   {
      T_T(std::string("Error opening file: ") + dataPath);
   }
   
   // Read in the JSON data in the file
   Json::Value jsonRoot;
   input >> jsonRoot;
   
   if(jsonRoot.isNull())
   {
      DEBUG("Unexpected root element name.");
      T_T("Failed to parse spritesheet data.");
   }
   
   parseFrames(jsonRoot);
   parseAnimations(jsonRoot);

   DEBUG("Spritesheet constructed!");
}

void Spritesheet::parseFrames(Json::Value& rootElement)
{
   // Get the frames array in the spritesheet data
   Json::Value& framesElement = rootElement["frames"];
   numFrames = framesElement.size();
   
   // This spritesheet is well-formed only if the "frames" element is a non-empty array
   // (i.e. there are frames in the spritesheet)
   if(!framesElement.isArray() || numFrames <= 0)
   {
      DEBUG("No frames found in spritesheet.");
      T_T("Empty (invalid) spritesheet constructed.");
   }

   DEBUG("Loading frames...");
   frameList.reserve(numFrames);
   for(int i = 0; i < numFrames; ++i)
   {
      Json::Value& currFrame = framesElement[i];
      
      // Get the current frame name
      std::string frameName = currFrame["name"].asString();
      if(frameName == UNTITLED_LINE)
      {
         // Skip untitled frames.
         continue;
      }
      
      // Make sure this frame name has not already been used in this file
      if(frameIndices.find(frameName) != frameIndices.end())
      {
         DEBUG("Duplicated name %s in spritesheet.", frameName.c_str());
         T_T("Parse error reading spritesheet.");
      }
      else
      {
         DEBUG("Adding frame %s...", frameName.c_str());
      }

      // Get the frame rectangle coordinates
      const shapes::Rectangle rect(
            currFrame["top"].asInt(),
            currFrame["left"].asInt(),
            currFrame["bottom"].asInt(),
            currFrame["right"].asInt());

      frameList.push_back(rect);
      DEBUG("Frame %s loaded in with coordinates %d, %d, %d, %d",
            frameName.c_str(), rect.left, rect.top, rect.right, rect.bottom);

      frameIndices[frameName] = i;
   }

   DEBUG("Frames loaded.");
}

void Spritesheet::parseAnimations(Json::Value& rootElement)
{
   Json::Value& animsElement = rootElement["animations"];
   int numAnimations = animsElement.size();

   if(!animsElement.isArray() || numAnimations <= 0)
   {
      DEBUG("No animations found in spritesheet.");
      return;
   }
   
   for(int i = 0; i < numAnimations; ++i)
   {
      Json::Value& currAnimation = animsElement[i];

      // Get the name of the animation
      std::string animationName = currAnimation["name"].asString();
      if(animationName == UNTITLED_LINE)
      {
         // Skip untitled animations
         continue;
      }
      
      // Make sure this animation name has not already been used in this file
      if(animationList.find(animationName) != animationList.end())
      {
         DEBUG("Duplicated animation name %s in spritesheet.", animationName.c_str());
         T_T("Parse error reading spritesheet.");
      }
      
      // Get the frames of the animation
      FrameSequence* frameSequence = new FrameSequence();

      Json::Value& frameArray = currAnimation["frames"];
      int sequenceLength = frameArray.size();
      if(!frameArray.isArray() || sequenceLength <= 0)
      {
         // There should be no such thing as an animation without a non-empty array of frames
         DEBUG("Encountered malformed animation %s.", animationName.c_str());
         T_T("Parse error reading spritesheet.");
      }

      for(int i = 0; i < sequenceLength; ++i)
      {
         // Get the name of the next frame in the animation
         std::string frameName = frameArray[i].asString();

         // Ensure that the frame exists in the frame list and grab the associated frame index
         std::map<std::string, int>::const_iterator frameIndexIter = frameIndices.find(frameName);
         if(frameIndexIter == frameIndices.end())
         {
            DEBUG("Found invalid frame name '%s' in animation %s", frameName.c_str(), animationName.c_str());
            T_T("Parse error reading spritesheet.");
         }
         
         int frameIndex = frameIndexIter->second;

         DEBUG("Animation %s: Adding node with index %d", animationName.c_str(), frameIndex);

         // Add the retrieved frame index into the sequence of frames
         frameSequence->push_back(frameIndex);
      }

      // Bind the animation name to the next available animation index
      animationList[animationName] = frameSequence;
   }
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
   std::map<std::string, const FrameSequence*>::const_iterator animFrames = animationList.find(animationName);
   if(animFrames != animationList.end())
   {
      return new Animation(animationName, *(animFrames->second));
   }

   return NULL;
}


void Spritesheet::draw(const shapes::Point2D& point, const int frameIndex) const
{
   if(!isInitialized())
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

   const shapes::Rectangle& frame = frameList[frameIndex];

   /**
    * \todo We can do all these calculations when the frames are initialized
    *       in order to optimize the drawing code a bit more!
    */
   const int frameHeight = frame.getHeight();
   const int frameWidth = frame.getWidth();

   float frameTop = frame.top / float(size.height);
   float frameBottom = frame.bottom / float(size.height);
   float frameLeft = frame.left / float(size.width);
   float frameRight = frame.right / float(size.width);
   
   float destLeft = float(point.x);
   float destBottom = float(point.y);
   float destRight = destLeft + frameWidth;
   float destTop = destBottom - frameHeight;

   // NOTE: Alpha testing doesn't do transparency; it either draws a pixel or it doesn't
   // If we want partial transparency, we would need to use alpha blending

   // Retain old alpha test state, function and threshold
   glPushAttrib(GL_COLOR_BUFFER_BIT);

   // Enable alpha testing
   glEnable(GL_ALPHA_TEST);

   // Set the alpha blending evaluation function
   glAlphaFunc(GL_GREATER, 0.1f);

   texture->bind();

   glBegin(GL_QUADS);
      glTexCoord2f(frameLeft, frameTop); glVertex3f(destLeft, destTop, 0.0f);
      glTexCoord2f(frameRight, frameTop); glVertex3f(destRight, destTop, 0.0f);
      glTexCoord2f(frameRight, frameBottom); glVertex3f(destRight, destBottom, 0.0f);
      glTexCoord2f(frameLeft, frameBottom); glVertex3f(destLeft, destBottom, 0.0f);
   glEnd();

   // We're done with alpha testing, return to default state
   glPopAttrib();
}

size_t Spritesheet::getResourceSize() const
{
   return sizeof(Spritesheet);
}

Spritesheet::~Spritesheet()
{
   // Delete the frame lists for the animations
   std::map<std::string, const FrameSequence*>::iterator iter;
   for(iter = animationList.begin(); iter != animationList.end(); ++iter)
   {
      delete iter->second;
   }

   if(texture != NULL)
   {
      delete texture;
   }
}
