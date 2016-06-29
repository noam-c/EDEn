/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "Size.h"

namespace geometry
{
   unsigned int Size::getArea() const noexcept
   {
      return width * height;
   }

   bool Size::operator==(const Size& rhs) const noexcept
   {
      return width == rhs.width && height == rhs.height;
   }

   bool Size::operator!=(const Size& rhs) const noexcept
   {
      return width != rhs.width || height != rhs.height;
   }

   Size Size::operator*(int coeff) const
   {
      return Size(width * coeff, height * coeff);
   }

   Size& Size::operator*=(int coeff) noexcept
   {
      width *= coeff;
      height *= coeff;
      return *this;
   }

   Size Size::operator/(int coeff) const
   {
      return Size(width / coeff, height / coeff);
   }

   Size& Size::operator/=(int coeff) noexcept
   {
      width /= coeff;
      height /= coeff;
      return *this;
   }
};
