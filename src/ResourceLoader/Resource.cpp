/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "Resource.h"

Resource::Resource(const ResourceKey& name) :
   m_name(name)
{
}

Resource::~Resource() = default;

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
