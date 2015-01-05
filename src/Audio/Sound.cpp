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

std::map<int, std::weak_ptr<Sound>> Sound::playingList;

void Sound::channelFinished(int channel)
{
   DEBUG("Channel %d finished playing.", channel);
   auto finishedSound = Sound::playingList[channel].lock();
   if(finishedSound)
   {
      finishedSound->finished();
   }

   Sound::playingList[channel].reset();
}

Sound::Sound(ResourceKey name) :
   Resource(name),
   m_sound(nullptr, &Mix_FreeChunk),
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
   m_sound.reset(Mix_LoadWAV(path.c_str()));

   if(!m_sound)
   {
      T_T(Mix_GetError());
   }

   DEBUG("Successfully loaded WAV %s.", path.c_str());
}

void Sound::play(const std::shared_ptr<Task>& task)
{
   if(!Settings::getCurrentSettings().isSoundEnabled() || !m_sound)
   {
      if(task)
      {
         task->complete();
      }

      return;
   }

   m_playingChannel = Mix_PlayChannel(-1, m_sound.get(), 0);
   if(m_playingChannel == -1)
   {
      DEBUG("There was a problem playing the sound ""%s"": %s", getResourceName().c_str(), Mix_GetError());
   }

   Sound::playingList[m_playingChannel] = std::static_pointer_cast<Sound>(shared_from_this());
   m_playTask = task;
}

void Sound::stop()
{
   if(m_playingChannel >= 0)
   {
      auto soundInChannel = Sound::playingList[m_playingChannel].lock();
      if(soundInChannel && this == soundInChannel.get())
      {
         Mix_HaltChannel(m_playingChannel);
      }
   }
}

void Sound::finished()
{
   DEBUG("Sound finished.");
   if(m_playTask)
   {
      m_playTask->complete();
      m_playTask.reset();
   }

   m_playingChannel = -1;
}

Sound::~Sound()
{
   DEBUG("Deleting sound: %s", getResourceName().c_str());
   stop();
}
