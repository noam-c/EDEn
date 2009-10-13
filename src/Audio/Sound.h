#ifndef __SOUND_H_
#define __SOUND_H_

#include "Resource.h"
#include "SDL_mixer.h"
#include <map>

class Task;

/** \todo Get rid of the SOUND_OFF macro usage once an options menu with
 * persistence is successfully implemented.
 */
// Uncomment this macro to turn off sound
//#define SOUND_OFF
class Sound : public Resource
{
   static std::map<int, Sound*> playingList;
   static bool ownsChannel(Sound* sound, int channel);
   static void channelFinished(int channel);

   Task* playTask;
   Mix_Chunk* sound;
   int playingChannel;

   void load(const char* path);

   void finished();

   public:
      Sound(ResourceKey name);
      void play(Task* task = NULL);
      void stop();
      size_t getSize();
      ~Sound();
};

#endif
