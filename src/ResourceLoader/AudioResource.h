#ifndef __AUDIO_RESOURCE_H_
#define __AUDIO_RESOURCE_H_

#include "Resource.h"
#include "SDL_mixer.h"

class Music : public Resource
{  Mix_Music* music;

   public:
      Music(char* path);
      size_t getSize();
      Mix_Music* getMusic();
      ~Music();
};

class Sound : public Resource
{  Mix_Chunk* sound;

   public:
      Sound(char* path);
      size_t getSize();
      Mix_Chunk* getSound();
      ~Sound();
};

#endif
