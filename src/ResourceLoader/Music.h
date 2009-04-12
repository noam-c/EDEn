#ifndef __MUSIC_H_
#define __MUSIC_H_

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

#endif
