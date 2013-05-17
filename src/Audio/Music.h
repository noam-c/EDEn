/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef MUSIC_H
#define MUSIC_H

#include "Resource.h"
#include "SDL_mixer.h"

/**
 * This Resource represents a song, and provides an interface for playing,
 * fading, or looping a song.
 *
 * @author Noam Chitayat
 */
class Music : public Resource
{
   /** The current music that is playing. */
   static Music* currentMusic;

   /**
    * @param music The music to check.
    *
    * @return true iff the music specified is the music currently playing
    */
   static inline bool isPlaying(Music* music);

   /**
    * Sets the currently playing music.
    *
    * @param music The music to play.
    */
   static inline void setPlayingMusic(Music* music);

   /** The SDL music object for this music resource */
   Mix_Music* music;

   /**
    * Loads the music resource with the specified file name and path.
    *
    * @param path The path to the music file.
    */
   void load(const std::string& path);

   public:
      /**
       * Constructor.
       *
       * @param name The name of the music Resource.
       */
      Music(ResourceKey name);

      /**
       * Play this song.
       */
      void play();

      /**
       * @return true iff there is music currently being played
       */
      static bool isPlaying();

      /**
       * Fades out the currently playing song.
       *
       * @param time The fadeout time (in ms).
       */
      static void fadeOutMusic(int time);

      /**
       * Immediately stops the currently playing song.
       */
      static void stopMusic();

      /**
       * Destructor.
       */
      ~Music();
};

#endif
