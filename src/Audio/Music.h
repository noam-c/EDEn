#ifndef __MUSIC_H_
#define __MUSIC_H_

#include "Resource.h"
#include "SDL_mixer.h"

class Music : public Resource
{  static Music* currentMusic;
   static inline bool isPlaying(Music* music);
   static inline void setPlayingMusic(Music* music);
   
   Mix_Music* music;

   void load(const char* path);

   public:
      Music(ResourceKey name);
      size_t getSize();

      void play();
      static void fadeOutMusic(int time);
      static void stopMusic();

      ~Music();
};

#endif
