/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "Settings.h"
#include <fstream>
#include "json/json.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_SETTINGS;

const std::string Settings::DEFAULT_SETTINGS_PATH = "settings.ini";
Settings Settings::currentSettings;

Settings& Settings::getCurrentSettings()
{
   return Settings::currentSettings;
}

void Settings::setCurrentSettings(const Settings& other)
{
   Settings::currentSettings = other;
}

void Settings::initialize()
{
   std::ifstream inputFile(Settings::DEFAULT_SETTINGS_PATH.c_str());
   
   if(inputFile)
   {
      Settings::currentSettings.load(inputFile);
   }
   else
   {
      Settings::currentSettings.createNewSettingsFile();
   }
}

Settings::Settings(bool isSnapshot) :
   settingsSnapshot(isSnapshot ? NULL : new Settings(true)),
   musicEnabled(true),
   soundEnabled(true),
   fullScreenEnabled(false),
   resolution(1024, 768, 32)
{
   
}

Settings::Settings(const Settings& other) :
   settingsSnapshot(other.settingsSnapshot),
   musicEnabled(other.musicEnabled),
   soundEnabled(other.soundEnabled),
   fullScreenEnabled(other.fullScreenEnabled),
   resolution(other.resolution)
{
   
}

Settings& Settings::operator=(const Settings& other)
{
   if(this != &other)
   {
      musicEnabled = other.musicEnabled;
      soundEnabled = other.soundEnabled;
      fullScreenEnabled = other.fullScreenEnabled;
      resolution = other.resolution;
   }
   
   return *this;
}

Settings::~Settings()
{
   if(settingsSnapshot != NULL)
   {
      delete settingsSnapshot;
   }
}

void Settings::createNewSettingsFile()
{
   save();
}

void Settings::save()
{
   std::ofstream outputFile(Settings::DEFAULT_SETTINGS_PATH.c_str());
   save(outputFile);
}

void Settings::save(std::ostream& output)
{
   if(!output)
   {
      T_T("Failed to write settings data.");
   }
   
   Json::Value jsonRoot;
   
   jsonRoot["musicEnabled"] = Settings::musicEnabled;
   jsonRoot["soundEnabled"] = Settings::soundEnabled;
   jsonRoot["fullScreenEnabled"] = Settings::fullScreenEnabled;
   
   Json::Value& resolutionSettings = jsonRoot["resolution"] = Json::Value(Json::objectValue);
   resolutionSettings["bitsPerPixel"] = Settings::resolution.getBitsPerPixel();
   resolutionSettings["height"] = Settings::resolution.getHeight();
   resolutionSettings["width"] = Settings::resolution.getWidth();
   
   output << jsonRoot;

   *settingsSnapshot = *this;
}

void Settings::load(std::istream& input)
{
   if(!input)
   {
      T_T("Failed to read settings data.");
   }
   
   Json::Value jsonRoot;
   input >> jsonRoot;
   
   if(jsonRoot.isNull())
   {
      DEBUG("Unexpected root element name.");
      T_T("Failed to parse settings.");
   }
   
   musicEnabled = jsonRoot.get("musicEnabled", true).asBool();
   soundEnabled = jsonRoot.get("soundEnabled", true).asBool();
   fullScreenEnabled = jsonRoot.get("fullScreenEnabled", true).asBool();

   Json::Value& resolutionSettings = jsonRoot["resolution"];
   unsigned int resolutionBitsPerPixel = resolutionSettings.get("bitsPerPixel", 32).asUInt();
   unsigned int resolutionHeight = resolutionSettings.get("height", 768).asUInt();
   unsigned int resolutionWidth = resolutionSettings.get("width", 1024).asUInt();
   
   resolution = Settings::Resolution(resolutionWidth, resolutionHeight, resolutionBitsPerPixel);

   *settingsSnapshot = *this;
}

void Settings::revertVideoChanges()
{
   resolution = settingsSnapshot->resolution;
   fullScreenEnabled = settingsSnapshot->fullScreenEnabled;
}

void Settings::revertChanges()
{
   *this = *settingsSnapshot;
}

bool Settings::isMusicEnabled() const
{
   return musicEnabled;
}

void Settings::setMusicEnabled(bool value)
{
   musicEnabled = value;
}

bool Settings::isSoundEnabled() const
{
   return soundEnabled;
}

void Settings::setSoundEnabled(bool value)
{
   soundEnabled = value;
}

bool Settings::isFullScreenEnabled() const
{
   return fullScreenEnabled;
}

void Settings::setFullScreenEnabled(bool value)
{
   fullScreenEnabled = value;
}

const Settings::Resolution& Settings::getResolution() const
{
   return resolution;
}

void Settings::setResolution(const Settings::Resolution& value)
{
   resolution = value;
}
