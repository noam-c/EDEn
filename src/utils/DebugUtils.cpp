/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include "DebugUtils.h"
#include <stdio.h>
#include <stdarg.h>

long DebugUtils::debugFlags = DEBUG_ALL;

void DebugUtils::print(long flag, std::string str) noexcept
{
   if(debugFlags & flag) {
      fprintf(stderr, "%s", str.c_str());
   }
}

void DebugUtils::print(long flag, const char* fmt, ...) noexcept
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

void DebugUtils::pause() noexcept
{
   printf("Press enter to continue...\n");
   getchar();
}
