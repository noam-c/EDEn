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
   m_settingsSnapshot(isSnapshot ? NULL : new Settings(true)),
   m_musicEnabled(true),
   m_soundEnabled(true),
   m_fullScreenEnabled(false),
   m_resolution(1024, 768, 32)
{
   
}

Settings::Settings(const Settings& other) :
   m_settingsSnapshot(other.m_settingsSnapshot),
   m_musicEnabled(other.m_musicEnabled),
   m_soundEnabled(other.m_soundEnabled),
   m_fullScreenEnabled(other.m_fullScreenEnabled),
   m_resolution(other.m_resolution)
{
   
}

Settings& Settings::operator=(const Settings& other)
{
   if(this != &other)
   {
      m_musicEnabled = other.m_musicEnabled;
      m_soundEnabled = other.m_soundEnabled;
      m_fullScreenEnabled = other.m_fullScreenEnabled;
      m_resolution = other.m_resolution;
   }
   
   return *this;
}

Settings::~Settings()
{
   if(m_settingsSnapshot != NULL)
   {
      delete m_settingsSnapshot;
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
   
   jsonRoot["musicEnabled"] = m_musicEnabled;
   jsonRoot["soundEnabled"] = m_soundEnabled;
   jsonRoot["fullScreenEnabled"] = m_fullScreenEnabled;
   
   Json::Value& resolutionSettings = jsonRoot["resolution"] = Json::Value(Json::objectValue);
   resolutionSettings["bitsPerPixel"] = m_resolution.getBitsPerPixel();
   resolutionSettings["height"] = m_resolution.getHeight();
   resolutionSettings["width"] = m_resolution.getWidth();
   
   output << jsonRoot;

   *m_settingsSnapshot = *this;
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
   
   m_musicEnabled = jsonRoot.get("musicEnabled", true).asBool();
   m_soundEnabled = jsonRoot.get("soundEnabled", true).asBool();
   m_fullScreenEnabled = jsonRoot.get("fullScreenEnabled", true).asBool();

   Json::Value& resolutionSettings = jsonRoot["resolution"];
   unsigned int resolutionBitsPerPixel = resolutionSettings.get("bitsPerPixel", 32).asUInt();
   unsigned int resolutionHeight = resolutionSettings.get("height", 768).asUInt();
   unsigned int resolutionWidth = resolutionSettings.get("width", 1024).asUInt();
   
   m_resolution = Settings::Resolution(resolutionWidth, resolutionHeight, resolutionBitsPerPixel);

   *m_settingsSnapshot = *this;
}

void Settings::revertVideoChanges()
{
   m_resolution = m_settingsSnapshot->m_resolution;
   m_fullScreenEnabled = m_settingsSnapshot->m_fullScreenEnabled;
}

void Settings::revertChanges()
{
   *this = *m_settingsSnapshot;
}

bool Settings::isMusicEnabled() const
{
   return m_musicEnabled;
}

void Settings::setMusicEnabled(bool value)
{
   m_musicEnabled = value;
}

bool Settings::isSoundEnabled() const
{
   return m_soundEnabled;
}

void Settings::setSoundEnabled(bool value)
{
   m_soundEnabled = value;
}

bool Settings::isFullScreenEnabled() const
{
   return m_fullScreenEnabled;
}

void Settings::setFullScreenEnabled(bool value)
{
   m_fullScreenEnabled = value;
}

const Settings::Resolution& Settings::getResolution() const
{
   return m_resolution;
}

void Settings::setResolution(const Settings::Resolution& value)
{
   m_resolution = value;
}
