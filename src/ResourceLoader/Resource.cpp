/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#include "Resource.h"

Resource::Resource(const ResourceKey& name) : initialized(false), name(name)
{
}

bool Resource::isInitialized()
{
   return initialized;
}

void Resource::initialize(const char* path)
{
   load(path);
   initialized = true;
}

std::string Resource::getResourceName()
{
   return std::string(name);
}

Resource::~Resource()
{
}
