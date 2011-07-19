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
