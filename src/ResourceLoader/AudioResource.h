#ifndef __AUDIO_RESOURCE_H_
#define __AUDIO_RESOURCE_H_

#include "Resource.h"
#include "SDL_mixer.h"

class Music : public Resource
{  static Music* currentMusic;
   static inline bool isPlaying(Music* music);
   static inline void setPlayingMusic(Music* music);
   
   Mix_Music* music;
   public:
      Music(const char* path);
      size_t getSize();

      void play();
      void fadeOut(int time);
      void stop();

      ~Music();
};

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
