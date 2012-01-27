/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "Label.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_EDWT;

namespace edwt
{
   Label::Label() : gcn::Label()
   {
   }

   Label::Label(const std::string& caption) : gcn::Label(caption)
   {
   }
};
