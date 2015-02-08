/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#ifndef MUSIC_H
#define MUSIC_H

#include "Resource.h"
#include "SDL_mixer.h"
#include <memory>

/**
 * This Resource represents a song, and provides an interface for playing,
 * fading, or looping a song.
 *
 * @author Noam Chitayat
 */
class Music final : public Resource
{
   /** The current music that is playing. */
   static std::shared_ptr<Music> currentMusic;

   /**
    * @return true iff this music is the music currently playing
    */
   inline bool isCurrentlyPlayingMusic() const;

   /** The SDL music object for this music resource */
   std::unique_ptr<Mix_Music, void(*)(Mix_Music*)> m_music;

   /**
    * Loads the music resource with the specified file name and path.
    *
    * @param path The path to the music file.
    */
   void load(const std::string& path) override;

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
      static bool isMusicPlaying();

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
      ~Music() override;
};

#endif
