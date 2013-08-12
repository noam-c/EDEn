/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "Sound.h"
#include "Settings.h"
#include "Task.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_AUDIO;

std::map<int, Sound*> Sound::playingList;

bool Sound::ownsChannel(Sound* sound, int channel)
{
   return sound == Sound::playingList[channel];
}

void Sound::channelFinished(int channel)
{
   DEBUG("Channel %d finished playing.", channel);
   Sound* finishedSound = Sound::playingList[channel];

   if(finishedSound != NULL)
   {  finishedSound->finished();
   }
}

Sound::Sound(ResourceKey name) :
   Resource(name),
   m_playingChannel(-1)
{
}

void Sound::load(const std::string& path)
{
   /**
    * \todo This should only be called once. Move it into initialization code.
    */
   Mix_ChannelFinished(&Sound::channelFinished);

   DEBUG("Loading WAV %s", path.c_str());
   m_sound = Mix_LoadWAV(path.c_str());

   if(m_sound == NULL)
   {
      T_T(Mix_GetError());
   }

   DEBUG("Successfully loaded WAV %s.", path.c_str());
}

void Sound::play(Task* task)
{
   if(!Settings::getCurrentSettings().isSoundEnabled() || m_sound == NULL)
   {
      if(task)
      {
         task->signal();
      }

      return;
   }

   m_playingChannel = Mix_PlayChannel(-1, m_sound, 0);
   if(m_playingChannel == -1)
   {
      DEBUG("There was a problem playing the sound ""%s"": %s", getResourceName().c_str(), Mix_GetError());
   }

   Sound::playingList[m_playingChannel] = this;
   m_playTask = task;
}

void Sound::stop()
{
   if(ownsChannel(this, m_playingChannel))
   {
      Mix_HaltChannel(m_playingChannel);
   }
}

void Sound::finished()
{
   DEBUG("Sound finished.");
   if(m_playTask)
   {
      m_playTask->signal();
      m_playTask = NULL;
   }

   m_playingChannel = -1;
}

Sound::~Sound()
{
   if(m_sound != NULL)
   {
      if(ownsChannel(this, m_playingChannel))
      {
         stop();
      }

      Mix_FreeChunk(m_sound);
   }
}
