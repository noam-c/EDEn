/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "Region.h"
#include "Map.h"
#include <fstream>
#include <dirent.h>
#include "DebugUtils.h"

#define DEBUG_FLAG DEBUG_RES_LOAD

Region::Region(const ResourceKey& name) :
   Resource(name),
   m_regionName(name)
{
}

Region::~Region() = default;

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

   for(const auto& filename : files)
   {
      std::string mapFile(std::string(path) + filename);
      try
      {
         auto nextMap = std::make_shared<Map>(filename.substr(0, filename.length() - 4), mapFile);
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
   return m_regionName;
}

std::weak_ptr<Map> Region::getStartingMap()
{
   return m_areas.begin()->second;
}

std::weak_ptr<Map> Region::getMap(const std::string& name)
{
   return m_areas[name];
}
