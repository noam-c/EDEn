/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef SOUND_H
#define SOUND_H

#include "Resource.h"
#include "SDL_mixer.h"
#include <map>

class Task;

/** \todo Get rid of the SOUND_OFF macro usage once an options menu with
 * persistence is successfully implemented.
 */
// Define this macro as 1 to turn off sound
#define SOUND_OFF 0

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

   /**
    * Checks whether or not a specified sound owns a certain channel.
    *
    * @param sound The sound to check.
    * @param channel The channel to check ownership of.
    *
    * @return true iff sound is playing on channel.
    */
   static bool ownsChannel(Sound* sound, int channel);

   /**
    * A callback used when a channel is released and its sound is done playing.
    *
    * @param channel The channel that has finished playing a sound.
    */
   static void channelFinished(int channel);

   /** A Task object used to signal waiting coroutines when this sound object is done playing. */
   Task* playTask;

   /** The SDL sound resource. */
   Mix_Chunk* sound;

   /** This sound's current channel. */
   int playingChannel;

   /**
    * Loads the music resource with the specified file name and path.
    *
    * @param path The path to the music file.
    */
   void load(const std::string& path);

   /**
    * A callback used to signal to the sound that it has finished playing.
    */
   void finished();

   public:
      /**
       * Constructor.
       *
       * @param name The name of the sound Resource.
       */
      Sound(ResourceKey name);

      /**
       * Play this sound once.
       *
       * @param task A task to signal when the sound completes. (optional)
       */
      void play(Task* task = NULL);

      /**
       * Stop this sound if it is currently playing.
       */
      void stop();

      /**
       * Destructor.
       */
      ~Sound();
};

#endif
