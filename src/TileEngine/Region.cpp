/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "Region.h"
#include "Map.h"
#include <fstream>
#include <dirent.h>
#include "DebugUtils.h"

const int debugFlag = DEBUG_RES_LOAD;

Region::Region(const ResourceKey& name) :
   Resource(name),
   m_name(name)
{
}

void Region::load(const std::string& path)
{
   struct dirent *entry;
   DIR *dp;
   std::vector<std::string> files;
   
   dp = opendir(path.c_str());
   if (dp == nullptr)
   {
      T_T("opendir");
   }
   
   while((entry = readdir(dp)))
   {
      std::string filename(entry->d_name);
      if(filename.length() > 4 && filename.substr(filename.length() - 4, 4) == ".tmx")
      {
         files.push_back(filename);
      }
   }
   
   closedir(dp);
   
   for(std::vector<std::string>::iterator iter = files.begin(); iter != files.end(); ++iter)
   {
      std::string mapFile(std::string(path) + *iter);
      try
      {
         auto nextMap = std::make_shared<Map>(iter->substr(0, iter->length() - 4), mapFile);
         m_areas[nextMap->getName()] = nextMap;
      }
      catch(Exception& e)
      {
         T_T(std::string("Malformed map in map file: ") + mapFile + '\n' + e.getMessage());
      }
   }
}

std::string Region::getName() const
{
   return m_name;
}

std::weak_ptr<Map> Region::getStartingMap()
{
   return m_areas.begin()->second;
}

std::weak_ptr<Map> Region::getMap(const std::string& name)
{
   return m_areas[name];
}
