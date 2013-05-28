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

Settings::Resolution::Resolution(unsigned int width, unsigned int height, unsigned int bitsPerPixel) :
   height(height),
   width(width),
   bitsPerPixel(bitsPerPixel)
{
}

unsigned int Settings::Resolution::getHeight() const
{
   return height;
}

unsigned int Settings::Resolution::getWidth() const
{
   return width;
}

unsigned int Settings::Resolution::getBitsPerPixel() const
{
   return bitsPerPixel;
}

Settings& Settings::getCurrentSettings()
{
   return currentSettings;
}

void Settings::initialize()
{
   std::ifstream inputFile(Settings::DEFAULT_SETTINGS_PATH.c_str());
   
   if(inputFile)
   {
      currentSettings.load(inputFile);
   }
   else
   {
      currentSettings.createNewSettingsFile();
   }
}

Settings::Settings() :
   musicEnabled(true),
   soundEnabled(true),
   fullScreenEnabled(false),
   resolution(1024, 768, 32)
{
   
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
   
   Settings::musicEnabled = jsonRoot.get("musicEnabled", true).asBool();
   Settings::soundEnabled = jsonRoot.get("soundEnabled", true).asBool();
   Settings::fullScreenEnabled = jsonRoot.get("fullScreenEnabled", true).asBool();

   Json::Value& resolutionSettings = jsonRoot["resolution"];
   unsigned int resolutionBitsPerPixel = resolutionSettings.get("bitsPerPixel", 32).asUInt();
   unsigned int resolutionHeight = resolutionSettings.get("height", 768).asUInt();
   unsigned int resolutionWidth = resolutionSettings.get("width", 1024).asUInt();
   
   Settings::resolution = Settings::Resolution(resolutionWidth, resolutionHeight, resolutionBitsPerPixel);
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
