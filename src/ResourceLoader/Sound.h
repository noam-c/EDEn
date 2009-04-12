#ifndef __SOUND_H_
#define __SOUND_H_

#include "Resource.h"
#include "SDL_mixer.h"
#include <map>

class Sound : public Resource
{  static std::map<int, Sound*> playingList;
   static bool ownsChannel(Sound* sound, int channel);
   static void channelFinished(int channel);

   Mix_Chunk* sound;
   int playingChannel;

   void finished();

   public:
      Sound(const char* path);
      void play();
      void stop();
      size_t getSize();
      ~Sound();
};

#endif
