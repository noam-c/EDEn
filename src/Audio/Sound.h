/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#ifndef SOUND_H
#define SOUND_H

#include "Resource.h"
#include "SDL_mixer.h"
#include <atomic>
#include <map>
#include <mutex>

class Task;

/**
 * This Resource represents a sound, and provides an interface for playing
 * or stopping a sound.
 *
 * @author Noam Chitayat
 */
class Sound : public Resource
{
   /** The map of currently playing Sound resources based on channel. */
   static std::map<int, Sound*> playingList;

   /** The mutex used to synchronize use of playingList. */
   static std::mutex playingListMutex;
   
   /** A Task object used to signal waiting coroutines when this sound object is done playing. */
   std::shared_ptr<Task> m_playTask;

   /** The SDL sound resource. */
   std::unique_ptr<Mix_Chunk, void(*)(Mix_Chunk*)> m_sound;

   /** This sound's current channel. */
   std::atomic_int m_playingChannel;

   /** The mutex used to synchronize this Sound's resources. */
   std::mutex m_playbackMutex;

   /**
    * Loads the music resource with the specified file name and path.
    *
    * @param path The path to the music file.
    */
   void load(const std::string& path);

   /**
    * A callback used to signal to the sound that it has finished playing or that it has been
    * stopped (e.g. by calling Sound::stop or the Sound's destructor).
    * N.B.: This function is called by both the main thread and the SDL_Mixer thread.
    */
   void finished();

   public:
      /**
       * A callback used when a channel is released and its sound is done playing.
       *
       * @param channel The channel that has finished playing a sound.
       */
      static void channelFinished(int channel);

      /**
       * Constructor.
       *
       * @param name The name of the sound Resource.
       */
      Sound(ResourceKey name);

      /**
       * Destructor.
       */
      ~Sound();

      /**
       * Play this sound once.
       *
       * @param task A task to signal when the sound completes. (optional)
       */
      void play(const std::shared_ptr<Task>& task = nullptr);

      /**
       * Stop this sound if it is currently playing.
       */
      void stop();
};

#endif
