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

bool Settings::musicEnabled = true;
bool Settings::soundEnabled = true;

void Settings::load(const std::string filePath)
{
   DEBUG("Loading settings file %s", filePath.c_str());
   
   std::ifstream input(filePath.c_str());
   if(!input)
   {
      T_T("Failed to open settings file for reading.");
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
}

bool Settings::isMusicEnabled()
{
   return Settings::musicEnabled;
}

bool Settings::isSoundEnabled()
{
   return Settings::soundEnabled;
}
