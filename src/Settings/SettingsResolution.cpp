/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "Settings.h"

Settings::Resolution::Resolution(unsigned int width, unsigned int height, unsigned int bitsPerPixel) :
   height(height),
   width(width),
   bitsPerPixel(bitsPerPixel)
{
}

Settings::Resolution::Resolution(const Settings::Resolution& other) :
   height(other.height),
   width(other.width),
   bitsPerPixel(other.bitsPerPixel)
{
}

Settings::Resolution& Settings::Resolution::operator=(const Settings::Resolution& other)
{
   if(this != &other)
   {
      height = other.height;
      width = other.width;
      bitsPerPixel = other.bitsPerPixel;
   }
   
   return *this;
}

unsigned int Settings::Resolution::getHeight() const
{
   return height;
}

unsigned int Settings::Resolution::getWidth() const
{
   return width;
}

unsigned int Settings::Resolution::getBitsPerPixel() const
{
   return bitsPerPixel;
}
