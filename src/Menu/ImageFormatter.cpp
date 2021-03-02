/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "ImageFormatter.h"
#include <RmlUi/Core/TypeConverter.h>

ImageFormatter::ImageFormatter() :
   Rml::Controls::DataFormatter("image")
{
}

void ImageFormatter::FormatData(Rml::Core::String& formatted_data, const Rml::Core::StringList& raw_data)
{
   if(!raw_data.empty() && !raw_data[0].empty())
   {
      formatted_data = "<img class=\"datagridimage\" src=\"../" + raw_data[0] + "\" />";
   }
}
