#include "Resource.h"

Resource::Resource(ResourceKey name) : name(name)
{}

std::string Resource::getName()
{  return std::string(name);
}
