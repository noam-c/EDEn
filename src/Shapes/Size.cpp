/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "Size.h"

namespace shapes
{
   Size::Size() :
      width(0),
      height(0)
   {}
   
   Size::Size(unsigned int width, unsigned int height) :
      width(width),
      height(height)
   {}
   
   bool Size::operator==(const Size& rhs) const
   {
      return width == rhs.width && height == rhs.height;
   }
   
   bool Size::operator!=(const Size& rhs) const
   {
      return width != rhs.width || height != rhs.height;
   }
   
   Size Size::operator*(int coeff) const
   {
      return Size(width * coeff, height * coeff);
   }
   
   Size& Size::operator*=(int coeff)
   {
      width *= coeff;
      height *= coeff;
      return *this;
   }
   
   Size Size::operator/(int coeff) const
   {
      return Size(width / coeff, height / coeff);
   }
   
   Size& Size::operator/=(int coeff)
   {
      width /= coeff;
      height /= coeff;
      return *this;
   }
};
