/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#include "Region.h"
#include "Map.h"
#include <fstream>
#include "DebugUtils.h"

const int debugFlag = DEBUG_RES_LOAD;

Region::Region(const ResourceKey& name) : Resource(name), regionName(name)
{
}

void Region::load(const char* path)
{
   /**
    * \todo Regions shouldn't assume that files are well-formed.
    *       Find and report errors if the file isn't formed ideally.
    */

   std::ifstream in;
   in.open(path);
   if(!in.is_open())
   {
      T_T(std::string("Error opening file: ") + path);
   }

   in >> regionName;

   // Get rid of remaining crap on the line (like DOS newline characters...)
   std::string remainder;
   std::getline(in, remainder);

   if(!in)
   {
      T_T(std::string("Error reading from file: ") + path);
   }
   else if(in.eof())
   {
      T_T(std::string("Region file contains no maps: ") + path);
   }

   Map* nextMap;
   while(!in.eof())
   {
      try
      {
         nextMap = new Map(in);
         areas[nextMap->getName()] = nextMap;
      }
      catch(Exception e)
      {
         T_T(std::string("Malformed map in region file: ") + path + '\n' + e.getMessage());
      }
   }
}

std::string Region::getName()
{
   return regionName;
}

Map* Region::getStartingMap()
{
   return areas.begin()->second;
}

Map* Region::getMap(const std::string& name)
{
   return areas[name];
}

size_t Region::getSize()
{
   return sizeof(this);
}

Region::~Region()
{
   for(std::map<std::string, Map*>::iterator i = areas.begin(); i != areas.end(); ++i)
   {
      delete (i->second);
   }
}
