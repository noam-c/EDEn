#include "Music.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_AUDIO;

Music* Music::currentMusic = NULL;

Music::Music(const char* path)
{  music = Mix_LoadMUS(path);
   if(music == NULL)
   {  T_T(Mix_GetError());
   }
}

size_t Music::getSize()
{  return sizeof(Music);
}

void Music::setPlayingMusic(Music* music)
{  currentMusic = music;
}

bool Music::isPlaying(Music* music)
{  return (music == currentMusic) && Mix_PlayingMusic();
}

void Music::play()
{  if(!isPlaying(this))
   {  setPlayingMusic(this);
      Mix_PlayMusic(music, 0);
   }
}

void Music::fadeOut(int time)
{  if(isPlaying(this))
   {  Mix_FadeOutMusic(time);
   }
}

void Music::stop()
{  if(isPlaying(this))
   {  Mix_HaltMusic();
   }
}

Music::~Music()
{  Mix_FreeMusic(music);
}
