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
bool Settings::musicEnabled = true;
bool Settings::soundEnabled = true;
bool Settings::fullScreenEnabled = false;
unsigned int Settings::resolutionBitsPerPixel = 32;
unsigned int Settings::resolutionHeight = 768;
unsigned int Settings::resolutionWidth = 1024;

void Settings::initialize()
{
   std::ifstream inputFile(Settings::DEFAULT_SETTINGS_PATH.c_str());
   
   if(inputFile)
   {
      Settings::load(inputFile);
   }
   else
   {
      Settings::createNewSettingsFile();
   }
}

void Settings::createNewSettingsFile()
{
   std::ofstream outputFile(Settings::DEFAULT_SETTINGS_PATH.c_str());
   Settings::save(outputFile);
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
   resolutionSettings["bitsPerPixel"] = Settings::resolutionBitsPerPixel;
   resolutionSettings["height"] = Settings::resolutionHeight;
   resolutionSettings["width"] = Settings::resolutionWidth;
   
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
   Settings::resolutionBitsPerPixel = resolutionSettings.get("bitsPerPixel", 32).asUInt();
   Settings::resolutionHeight = resolutionSettings.get("height", 768).asUInt();
   Settings::resolutionWidth = resolutionSettings.get("width", 1024).asUInt();
}

bool Settings::isMusicEnabled()
{
   return Settings::musicEnabled;
}

bool Settings::isSoundEnabled()
{
   return Settings::soundEnabled;
}

bool Settings::isFullScreenEnabled()
{
   return Settings::fullScreenEnabled;
}

unsigned int Settings::getResolutionHeight()
{
   return Settings::resolutionHeight;
}

unsigned int Settings::getResolutionWidth()
{
   return Settings::resolutionWidth;
}

unsigned int Settings::getResolutionBitsPerPixel()
{
   return Settings::resolutionBitsPerPixel;
}
