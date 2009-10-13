#ifndef __MUSIC_H_
#define __MUSIC_H_

#include "Resource.h"
#include "SDL_mixer.h"

/** \todo Get rid of the MUSIC_OFF macro usage once an options menu with
 * persistence is successfully implemented.
 */
// Uncomment this macro to turn off music
//#define MUSIC_OFF

class Music : public Resource
{
   static Music* currentMusic;
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
