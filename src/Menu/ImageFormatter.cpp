#include "ImageFormatter.h"
#include <Rocket/Core/TypeConverter.h>

ImageFormatter::ImageFormatter() : Rocket::Controls::DataFormatter("image")
{
}

ImageFormatter::~ImageFormatter()
{
}

void ImageFormatter::FormatData(Rocket::Core::String& formatted_data, const Rocket::Core::StringList& raw_data)
{
   formatted_data = "<img style=\"height: 100px; width: 100px;\" src=\"../../" + raw_data[0] + "\" />";
}
