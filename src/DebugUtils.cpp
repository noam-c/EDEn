#include "DebugUtils.h"
#include <iostream>

long DebugUtils::debugFlags = ~0;

void DebugUtils::print(long flag, std::string str)
{  if(debugFlags & flag) std::cerr << str << std::endl;
}

void DebugUtils::print(long flag, int x)
{  if(debugFlags & flag) std::cerr << x << std::endl;
}
