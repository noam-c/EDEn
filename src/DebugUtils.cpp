#include "DebugUtils.h"
#include <stdio.h>
#include <stdarg.h>

long DebugUtils::debugFlags = ~0;

void DebugUtils::print(long flag, std::string str)
{
   if(debugFlags & flag) fprintf(stderr, "%s", str.c_str());
}

void DebugUtils::print(long flag, const char* fmt, ...)
{
   if(debugFlags & flag)
   {
      va_list argp;
      va_start(argp, fmt);
      vfprintf(stderr, fmt, argp);
      fprintf(stderr, "\n");
      va_end(argp);
   }
}
