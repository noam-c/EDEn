/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef IMAGE_FORMATTER_H
#define IMAGE_FORMATTER_H

#include <Rocket/Controls/DataFormatter.h>

class ImageFormatter : public Rocket::Controls::DataFormatter
{
   public:
      ImageFormatter();
      ~ImageFormatter();

      void FormatData(Rocket::Core::String& formatted_data, const Rocket::Core::StringList& raw_data);
};

#endif
