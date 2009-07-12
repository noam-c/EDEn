#include "Spritesheet.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "GraphicsUtil.h"
#include "TileEngine.h"
#include "Frame.h"
#include <queue>
#include <fstream>
#include <sstream>
#include "DebugUtils.h"

const int debugFlag = DEBUG_RES_LOAD | DEBUG_SPRITE;

const std::string Spritesheet::IMG_EXTENSION = ".png";
const std::string Spritesheet::DATA_EXTENSION = ".eds";

Spritesheet::Spritesheet(ResourceKey name, const char* path)
                                     : Resource(name), frameList(NULL), numFrames(0)
{  // Load the image itself into a texture using GraphicsUtil
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
   {  T_T(std::string("Error opening file: ") + dataPath);
   }

   std::string line;
   std::queue<Frame*> frameQueue;
   for(;;)
   {  // Get the next line
      getline(in, line);

      // If the file is done, we are done
      if(in.eof())
      {  break;
      }
      // If there is some other error, throw an exception
      else if(!in)
      {  T_T(std::string("Error reading from file: ") + path);
         break;
      }

      // Send the loaded line into a string stream for parsing
      std::string frameName;
      std::stringstream lineStream(line, std::stringstream::in | std::stringstream::out);

      // Get the name of the frame
      lineStream >> frameName;

      // Make sure this frame name has not already been used in this file
      if(frameIndices.find(frameName) != frameIndices.end())
      {  DEBUG("Duplicated name %s in spritesheet %s", frameName.c_str(), path);
         T_T("Parse error reading spritesheet.");
      }

      // Get the four coordinates of the frame
      int coords[4];
      for(int i = 0; i < 4; ++i)
      {  lineStream >> coords[i];
         if(!lineStream)
         {  DEBUG("Error reading frame %s in spritesheet %s", frameName.c_str(), path);
            T_T("Parse error reading spritesheet.");
         }
      }

      // Create a new frame with the read coordinates
      Frame* f = new Frame(coords[0], coords[1], coords[2], coords[3]);
      DEBUG("Frame %s loaded in with coordinates %d, %d, %d, %d",
                      frameName.c_str(), coords[0], coords[1], coords[2], coords[3]);

      // Bind the frame name to the next available frame index
      frameIndices[frameName] = frameQueue.size();

      // Push the frame into a queue for later processing
      frameQueue.push(f);

      // Flush any remaining line data in the stream
      /** \todo Should we be doing this? Or failing on extraneous text? */
      lineStream.flush();
   }

   // Create the new array of frames
   numFrames = frameQueue.size();
   frameList = new Frame*[numFrames];

   // Fill the new array with the accumulated frames
   int frameNum = 0;
   while(!frameQueue.empty())
   {  frameList[frameNum++] = frameQueue.front(); frameQueue.pop();
   }

   DEBUG("Spritesheet constructed!");
}

int Spritesheet::getFrameIndex(std::string frameName)
{  int index = frameIndices[frameName];

   if(index < 0 || index >= numFrames)
   {  return -1;
   }
}

void Spritesheet::draw(int x, int y, int frameIndex)
{  if(frameIndex < 0 || frameIndex > numFrames)
   {  T_T("Spritesheet frame index out of bounds!");
   }

   Frame f = *frameList[frameIndex];
   int frameHeight = f.bottom - f.top;
   int frameWidth = f.right - f.left;

   float destLeft = float(x);
   float destBottom = float(y + TileEngine::TILE_SIZE);
   float destRight = destLeft + frameWidth;
   float destTop = destBottom - frameHeight;

   float frameTop = f.top / float(height);
   float frameBottom = f.bottom / float(height);
   float frameLeft = f.left / float(width);
   float frameRight = f.right / float(width);

   glBindTexture(GL_TEXTURE_2D, texture);

   glBegin(GL_QUADS);
      glTexCoord2f(frameLeft, frameTop); glVertex3f(destLeft, destTop, 0.0f);
      glTexCoord2f(frameRight, frameTop); glVertex3f(destRight, destTop, 0.0f);
      glTexCoord2f(frameRight, frameBottom); glVertex3f(destRight, destBottom, 0.0f);
      glTexCoord2f(frameLeft, frameBottom); glVertex3f(destLeft, destBottom, 0.0f);
   glEnd();
}

size_t Spritesheet::getSize()
{  return sizeof(Spritesheet);
}
