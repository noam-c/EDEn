#include "Resource.h"

Resource::Resource(ResourceKey name) : name(name), initialized(false)
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
