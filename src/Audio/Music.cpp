/**
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#include "Music.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_AUDIO;

Music* Music::currentMusic = NULL;

Music::Music(ResourceKey name) : Resource(name)
{
}

void Music::load(const char* path)
{
   music = Mix_LoadMUS(path);

   if(music == NULL)
   {
      T_T(Mix_GetError());
   }
}

size_t Music::getSize()
{
   return sizeof(Music);
}

void Music::setPlayingMusic(Music* music)
{
   currentMusic = music;
}

bool Music::isPlaying(Music* music)
{
   return (music == currentMusic) && Mix_PlayingMusic();
}

void Music::fadeOutMusic(int time)
{
   if(Mix_PlayingMusic())
   {
      Mix_FadeOutMusic(time);
      currentMusic = NULL;
   }
}

void Music::stopMusic()
{
   if(Mix_PlayingMusic())
   {
      Mix_HaltMusic();
      currentMusic = NULL;
   }
}

void Music::play()
{
#ifndef MUSIC_OFF
   if(music == NULL) return;

   if(!isPlaying(this))
   {
      setPlayingMusic(this);
      if(Mix_PlayMusic(music, 0) < 0)
      {
         DEBUG("There was a problem playing the music: %s", Mix_GetError());
      }
   }
#endif
}

Music::~Music()
{
   if(music != NULL)
   {
      Mix_FreeMusic(music);
   }
}
