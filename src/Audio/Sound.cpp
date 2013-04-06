/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "Sound.h"
#include "Task.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_AUDIO;

std::map<int, Sound*> Sound::playingList;

bool Sound::ownsChannel(Sound* sound, int channel)
{
   return sound == playingList[channel];
}

void Sound::channelFinished(int channel)
{
   DEBUG("Channel %d finished playing.", channel);
   Sound* finishedSound = playingList[channel];

   if(finishedSound != NULL)
   {  finishedSound->finished();
   }
}

Sound::Sound(ResourceKey name) :
   Resource(name),
   playingChannel(-1)
{
}

void Sound::load(const std::string& path)
{
   /**
    * \todo This should only be called once. Move it into initialization code.
    */
   Mix_ChannelFinished(&Sound::channelFinished);

   DEBUG("Loading WAV %s", path.c_str());
   sound = Mix_LoadWAV(path.c_str());

   if(sound == NULL)
   {
      T_T(Mix_GetError());
   }

   DEBUG("Successfully loaded WAV %s.", path.c_str());
}

size_t Sound::getResourceSize() const
{
   return sizeof(Sound);
}

void Sound::play(Task* task)
{
   if(SOUND_OFF || sound == NULL)
   {
      if(task)
      {
         task->signal();
      }

      return;
   }

   playingChannel = Mix_PlayChannel(-1, sound, 0);
   if(playingChannel == -1)
   {
      DEBUG("There was a problem playing the sound ""%s"": %s", getResourceName().c_str(), Mix_GetError());
   }

   playingList[playingChannel] = this;
   playTask = task;
}

void Sound::stop()
{
   if(ownsChannel(this, playingChannel))
   {
      Mix_HaltChannel(playingChannel);
   }
}

void Sound::finished()
{
   DEBUG("Sound finished.");
   if(playTask)
   {
      playTask->signal();
      playTask = NULL;
   }

   playingChannel = -1;
}

Sound::~Sound()
{
   if(sound != NULL)
   {
      if(ownsChannel(this, playingChannel))
      {
         stop();
      }

      Mix_FreeChunk(sound);
   }
}
