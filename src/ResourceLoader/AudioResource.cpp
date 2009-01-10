#include "AudioResource.h"

#include "DebugUtils.h"

Music::Music(char* path)
{  music = Mix_LoadMUS(path);
   if(music == NULL)
   {  T_T(Mix_GetError());
   }
}

size_t Music::getSize()
{  return sizeof(Music);
}

Mix_Music* Music::getMusic()
{  return music;
}

Music::~Music()
{  Mix_FreeMusic(music);
}

Sound::Sound(char* path)
{  sound = Mix_LoadWAV(path);
   if(sound == NULL)
   {  T_T(Mix_GetError());
   }
}

size_t Sound::getSize()
{  return sizeof(Sound);
}

Mix_Chunk* Sound::getSound()
{  return sound;
}

Sound::~Sound()
{  Mix_FreeChunk(sound);
}

