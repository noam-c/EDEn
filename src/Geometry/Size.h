/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef SIZE_H
#define SIZE_H

namespace geometry
{
   struct Size
   {
      /** The width of the shape associated with this size. */
      unsigned int width = 0;

      /** The height of the shape associated with this size. */
      unsigned int height = 0;

      /**
       * Constructor.
       * Creates empty Size (width: 0, height: 0).
       */
      constexpr Size() = default;

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
       * Get the total area prescribed by this Size.
       */
      unsigned int getArea() const noexcept;

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
