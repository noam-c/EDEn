#include "AudioResource.h"
#include "DebugUtils.h"
#include <fstream>

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
{  std::ofstream out("stdout.txt");
   out << "Loading WAV " << path << std::endl;
   sound = Mix_LoadWAV(path);
   out << "Loaded WAV." << std::endl;
   if(sound == NULL)
   {  out << "Problem encountered: " << Mix_GetError() << std::endl;
      T_T(Mix_GetError());
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
