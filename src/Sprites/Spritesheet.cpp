#include "Spritesheet.h"
#include "Frame.h"
#include <fstream>
#include <sstream>
#include "DebugUtils.h"

const int debugFlag = DEBUG_RES_LOAD;

const std::string Spritesheet::IMG_EXTENSION = ".png";
const std::string Spritesheet::DATA_EXTENSION = ".eds";

Spritesheet::Spritesheet(ResourceKey name, const char* path) : Resource(name)
{  std::ifstream in;
   std::string dataPath(path);
   dataPath += DATA_EXTENSION;

   std::string imgPath(path);
   imgPath += IMG_EXTENSION;

   in.open(dataPath.c_str());
   if(!in.is_open())
   {  T_T(std::string("Error opening file: ") + dataPath);
   }

   std::string line;

   for(;;)
   {  getline(in, line);
      if(in.eof())
      {  break;
      }

      std::string frameName;
      std::stringstream lineStream(line, std::stringstream::in | std::stringstream::out);

      if(!in)
      {  T_T(std::string("Error reading from file: ") + path);
         break;
      }

      lineStream >> frameName;

      if(frames[frameName] != NULL)
      {  DEBUG("Duplicated name %s in spritesheet %s", frameName.c_str(), path);
         T_T("Parse error reading spritesheet.");
      }

      // This line contains a frame
      int coords[4];

      for(int i = 0; i < 4; ++i)
      {  lineStream >> coords[i];
         if(!lineStream)
         {  DEBUG("Error reading frame %s in spritesheet %s", frameName.c_str(), path);
            T_T("Parse error reading spritesheet.");
         }
      }

      Frame f(coords[0], coords[1], coords[2], coords[3]);

      DEBUG("Frame %s loaded in with coordinates %d, %d, %d, %d",
                      frameName.c_str(), coords[0], coords[1], coords[2], coords[3]);

      lineStream.flush();
   }
}

Frame* Spritesheet::getFrame(std::string frameName)
{  return frames[frameName];
}

void Spritesheet::draw(int x, int y, Frame* frame)
{  
}

size_t Spritesheet::getSize()
{  return sizeof(Spritesheet);
}
