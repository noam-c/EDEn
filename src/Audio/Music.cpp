/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "Music.h"
#include "Settings.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_AUDIO;

std::shared_ptr<Music> Music::currentMusic(nullptr);

Music::Music(ResourceKey name) :
   Resource(name),
   m_music(nullptr, &Mix_FreeMusic)
{
}

Music::~Music()
{
}

void Music::load(const std::string& path)
{
   m_music.reset(Mix_LoadMUS(path.c_str()));

   if(!m_music)
   {
      T_T(Mix_GetError());
   }
}

bool Music::isMusicPlaying()
{
   return Music::currentMusic && Mix_PlayingMusic();
}

bool Music::isCurrentlyPlayingMusic() const
{
   return Music::isMusicPlaying() && this == Music::currentMusic.get();
}

void Music::fadeOutMusic(int time)
{
   if(Mix_PlayingMusic())
   {
      if (time > 0)
      {
         Mix_FadeOutMusic(time);
      }
      else
      {
         Mix_HaltMusic();
      }

      Music::currentMusic.reset();
   }
}

void Music::stopMusic()
{
   fadeOutMusic(0);
}

void Music::play()
{
   if(!Settings::getCurrentSettings().isMusicEnabled() || !m_music)
   {
      return;
   }

   if(isCurrentlyPlayingMusic())
   {
      // This song is already the currently playing song,
      // so we're done here.
      return;
   }
   
   if(Mix_PlayMusic(m_music.get(), 0) < 0)
   {
      DEBUG("There was a problem playing the music: %s", Mix_GetError());
      Music::currentMusic.reset();
      return;
   }
   
   Music::currentMusic = std::static_pointer_cast<Music>(shared_from_this());
}
