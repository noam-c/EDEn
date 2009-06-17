#include "Resource.h"

Resource::Resource(ResourceKey name) : name(name)
{}

std::string Resource::getResourceName()
{  return std::string(name);
}
