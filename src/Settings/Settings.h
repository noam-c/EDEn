/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

/**
 * A static class that manages the game settings and options.
 *
 * @author Noam Chitayat
 */
class Settings
{
   /** True iff music should be played in the game. */
   static bool musicEnabled;

   /** True iff sound effects should be played in the game. */
   static bool soundEnabled;
   
   public:
      /**
       * Loads the user and game settings from a specified file.
       *
       * @param filePath The path to the settings file.
       */
      static void load(const std::string filePath = "settings.ini");

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
