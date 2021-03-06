/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <iostream>
#include <memory>

/**
 * A static class that manages the game settings and options.
 *
 * @author Noam Chitayat
 */
class Settings final
{
   const static std::string DEFAULT_SETTINGS_PATH;

   std::unique_ptr<Settings> m_settingsSnapshot;
   static Settings currentSettings;

   /** True iff music should be played in the game. */
   bool m_musicEnabled = true;

   /** True iff sound effects should be played in the game. */
   bool m_soundEnabled = true;

   /** True iff fullscreen mode is enabled in the game. */
   bool m_fullScreenEnabled = false;

   Settings(bool isSnapshot = false);

   /**
    * Creates a new settings file for the first run of the game.
    */
   void createNewSettingsFile();

   /**
    * Saves the user and game settings to a specified stream.
    *
    * @param output The output stream to save settings to.
    */
   void save(std::ostream& output);

   /**
    * Loads the user and game settings from a specified stream.
    *
    * @param input The input stream to load settings from.
    */
   void load(std::istream& input);

   public:
      struct Resolution final
      {
         /** The width of the game's window. */
         unsigned int width;

         /** The height of the game's window. */
         unsigned int height;

         /** The bit depth of the game's window. */
         unsigned int bitsPerPixel;
      };

   private:
      Resolution m_resolution = {1024, 768, 32};

   public:
      static void initialize();

      static Settings& getCurrentSettings();

      void setSettings(const Settings& other);

      /**
       * @return true iff the user has enabled music for the game.
       */
      bool isMusicEnabled() const;

      /**
       * Updates the settings to enable or disable music per the user's preferences.
       *
       * @param musicEnabled Set true iff the user has enabled music for the game.
       */
      void setMusicEnabled(bool musicEnabled);

      /**
       * @return true iff the user has enabled music for the game.
       */
      bool isSoundEnabled() const;

      /**
       * Updates the settings to enable or disable sound effects per the user's preferences.
       *
       * @param soundEnabled Set true iff the user has enabled sound for the game.
       */
      void setSoundEnabled(bool soundEnabled);

      /**
       * @return true iff the user has enabled fullscreen (non-windowed) mode for the game.
       */
      bool isFullScreenEnabled() const;

      /**
       * Updates the settings to enable or disable fullscreen rendering per the user's preferences.
       *
       * @param fullScreenEnabled Set true iff the user has enabled fullscreen rendering for the game.
       */
      void setFullScreenEnabled(bool fullScreenEnabled);

      /**
       * @return the resolution of the game window.
       */
      const Resolution& getResolution() const;

      void setResolution(const Resolution& resolution);

      /**
       * Saves the user and game settings to the default settings file.
       */
      void save();

      void revertVideoChanges();
      void revertChanges();
};

#endif
