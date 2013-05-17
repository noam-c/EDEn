/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <iostream>

/**
 * A static class that manages the game settings and options.
 *
 * @author Noam Chitayat
 */
class Settings
{
   const static std::string DEFAULT_SETTINGS_PATH;
   
   /** True iff music should be played in the game. */
   static bool musicEnabled;

   /** True iff sound effects should be played in the game. */
   static bool soundEnabled;

   /**
    * Creates a new settings file for the first run of the game.
    */
   static void createNewSettingsFile();
   
   /**
    * Saves the user and game settings to a specified stream.
    *
    * @param output The output stream to save settings to.
    */
   static void save(std::ostream& output);
   
   /**
    * Loads the user and game settings from a specified stream.
    *
    * @param input The input stream to load settings from.
    */
   static void load(std::istream& input);
   
   public:
      static void initialize();
   
      /**
       * @return true iff the user has enabled music for the game.
       */
      static bool isMusicEnabled();

      /**
       * @return true iff the user has enabled music for the game.
       */
      static bool isSoundEnabled();
};

#endif
