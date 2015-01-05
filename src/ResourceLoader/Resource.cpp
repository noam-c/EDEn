/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include "Resource.h"

Resource::Resource(const ResourceKey& name) :
   m_initialized(false),
   m_name(name)
{
}

bool Resource::isInitialized() const
{
   return m_initialized;
}

void Resource::initialize(const char* path)
{
   load(path);
   m_initialized = true;
}

std::string Resource::getResourceName() const
{
   return std::string(m_name);
}
