#include "Sound.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_AUDIO;

std::map<int, Sound*> Sound::playingList;

bool Sound::ownsChannel(Sound* sound, int channel)
{  return sound == playingList[channel];
}

void Sound::channelFinished(int channel)
{  Sound* finishedSound = playingList[channel];

   if(finishedSound != NULL)
   {  finishedSound->finished();
   }
}

Sound::Sound(const char* path) : playingChannel(-1)
{  DEBUG("Loading WAV %s", path);
   sound = Mix_LoadWAV(path);
   DEBUG("Loaded WAV.");
   if(sound == NULL)
   {  T_T(Mix_GetError());
   }
}

size_t Sound::getSize()
{  return sizeof(Sound);
}

void Sound::play()
{  playingChannel = Mix_PlayChannel(-1, sound, 0);
   if(playingChannel == -1)
   {  T_T(Mix_GetError());
   }

   Mix_ChannelFinished(&Sound::channelFinished);
}

void Sound::stop()
{  if(ownsChannel(this, playingChannel))
   {  Mix_HaltChannel(playingChannel);
   }
}

void Sound::finished()
{  playingChannel = -1;
}

Sound::~Sound()
{  if(ownsChannel(this, playingChannel))
   {  stop();
   }

   Mix_FreeChunk(sound);
}
