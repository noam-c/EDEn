/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#include "DebugUtils.h"
#include <stdio.h>
#include <stdarg.h>

long DebugUtils::debugFlags = DEBUG_ALL;

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

void DebugUtils::pause()
{
   printf("Press enter to continue...\n");
   getchar();
}
