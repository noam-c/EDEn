/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#include "TextField.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_EDWT;

namespace edwt
{
   TextField::TextField() : gcn::TextField()
   {
      adjustSize();
   }

   TextField::TextField(const std::string& text) : gcn::TextField(text)
   {
   }
};
