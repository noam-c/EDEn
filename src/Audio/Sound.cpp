/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "Sound.h"

#include "SDL_mixer.h"

#include "Settings.h"
#include "Task.h"

#include "DebugUtils.h"

#define DEBUG_FLAG DEBUG_AUDIO

std::map<int, Sound*> Sound::playingList;
std::mutex Sound::playingListMutex;

void Sound::channelFinished(int channel)
{
   DEBUG("Channel %d finished playing.", channel);

   Sound* soundToFinish = nullptr;

   {
      std::lock_guard<decltype(playingListMutex)> playingListLock(playingListMutex);

      auto finishedSoundIter = Sound::playingList.find(channel);
      if(finishedSoundIter != Sound::playingList.end())
      {
         std::swap(soundToFinish, finishedSoundIter->second);
      }
   }
   
   if(soundToFinish != nullptr)
   {
      soundToFinish->finished();
   }

   DEBUG("Channel %d: 'channelFinished' completed.", channel);
}

Sound::Sound(ResourceKey name) :
   Resource(name),
   m_sound(nullptr, &Mix_FreeChunk),
   m_playingChannel(-1)
{
}

Sound::~Sound()
{
   DEBUG("Sound \"%s\": Deleting...", getResourceName().c_str());
   stop();
}

void Sound::load(const std::string& path)
{
   std::lock_guard<decltype(m_playbackMutex)> lock(m_playbackMutex);

   DEBUG("Sound \"%s\": Loading WAV %s", getResourceName().c_str(), path.c_str());
   
   auto sound = Mix_LoadWAV(path.c_str());

   if(!sound)
   {
      T_T(Mix_GetError());
   }

   DEBUG("Sound \"%s\": Successfully loaded WAV %s.", getResourceName().c_str(), path.c_str());
   m_sound.reset(sound);
}

void Sound::play(const std::shared_ptr<Task>& task)
{
   std::lock_guard<decltype(m_playbackMutex)> playbackLock(m_playbackMutex);

   if(!Settings::getCurrentSettings().isSoundEnabled() || !m_sound)
   {
      if(task)
      {
         task->complete();
      }

      return;
   }

   // If the sound is already playing somewhere, stop it.
   stop();

   // At this point, there should be nothing playing.
   DEBUG("Sound \"%s\": Playing...", getResourceName().c_str());

   {
      std::lock_guard<decltype(playingListMutex)> playingListLock(playingListMutex);

      m_playingChannel = Mix_PlayChannel(-1, m_sound.get(), 0);
      if(m_playingChannel == -1)
      {
         DEBUG("There was a problem playing the sound ""%s"": %s", getResourceName().c_str(), Mix_GetError());
         task->complete();
         return;
      }

      DEBUG("Sound \"%s\": Using channel %d.", getResourceName().c_str(), m_playingChannel.load());

      Sound::playingList[m_playingChannel] = this;
      m_playTask = task;
   }
}

void Sound::stop()
{
   int playingChannel = m_playingChannel.load();

   bool ownsChannel = false;
   if(playingChannel != -1)
   {
      std::lock_guard<decltype(playingListMutex)> playingListLock(playingListMutex);
      auto soundInChannelIter = Sound::playingList.find(playingChannel);

      ownsChannel =
         soundInChannelIter != Sound::playingList.end() &&
         soundInChannelIter->second == this;
   }
   
   if(ownsChannel)
   {
      DEBUG("Sound \"%s\": Stopping...", getResourceName().c_str());

      // N.B.: This call will synchronously call the
      // Mix_ChannelFinished callback, so make sure that
      // path doesn't try to recursively acquire any locks
      // acquired at this point
      Mix_HaltChannel(playingChannel);

      DEBUG("Sound \"%s\": Stopped.", getResourceName().c_str());
   }
   else
   {
      m_playingChannel = -1;
   }
}

void Sound::finished()
{
   DEBUG("Sound \"%s\": Finished.", getResourceName().c_str());
   std::lock_guard<decltype(m_playbackMutex)> lock(m_playbackMutex);

   if(m_playTask)
   {
      m_playTask->complete();
      m_playTask.reset();
   }

   m_playingChannel = -1;
}
