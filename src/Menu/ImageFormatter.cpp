/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "ImageFormatter.h"
#include <Rocket/Core/TypeConverter.h>

ImageFormatter::ImageFormatter() :
   Rocket::Controls::DataFormatter("image")
{
}

void ImageFormatter::FormatData(Rocket::Core::String& formatted_data, const Rocket::Core::StringList& raw_data)
{
   if(!raw_data.empty() && !raw_data[0].Empty())
   {
      formatted_data = "<img class=\"datagridimage\" src=\"../../" + raw_data[0] + "\" />";
   }
}
