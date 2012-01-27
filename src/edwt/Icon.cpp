/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "Icon.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_EDWT;

namespace edwt
{
   Icon::Icon() : gcn::Icon()
   {
   }

   Icon::Icon(const std::string& filename) : gcn::Icon(filename)
   {
   }

   Icon::Icon(const gcn::Image* image) : gcn::Icon(image)
   {
   }

   void Icon::setImage(const std::string& filename)
   {
       gcn::Icon::setImage(gcn::Image::load(filename));
       mInternalImage = true;
   }

   void Icon::clearImage()
   {
        if (mInternalImage)
        {
            delete mImage;
        }

        mImage = NULL;
        mInternalImage = false;
        setSize(0, 0);
   }
};
