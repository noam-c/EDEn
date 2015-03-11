/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#ifndef SIZE_H
#define SIZE_H

namespace geometry
{
   struct Size
   {
      /** The width of the shape associated with this size. */
      unsigned int width;

      /** The height of the shape associated with this size. */
      unsigned int height;

      /**
       * Constructor.
       * Creates empty Size (width: 0, height: 0).
       */
      constexpr Size() :
         width(0),
         height(0)
      {}

      /**
       * Constructor.
       * Creates size of the specified width and height.
       *
       * @param width The width of the Size.
       * @param height The height of the Size.
       */
      constexpr Size(unsigned int width, unsigned int height) :
         width(width),
         height(height)
      {}

      /**
       * Equality operator.
       */
      bool operator==(const Size& rhs) const noexcept;

      /**
       * Inequality operator.
       */
      bool operator!=(const Size& rhs) const noexcept;

      /**
       * Multiplication operator.
       */
      Size operator*(int coeff) const;

      /**
       * Multiplication assignment operator.
       */
      Size& operator*=(int coeff) noexcept;

      /**
       * Division operator.
       */
      Size operator/(int coeff) const;

      /**
       * Division assignment operator.
       */
      Size& operator/=(int coeff) noexcept;
   };
};

#endif