/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef GRID_H
#define GRID_H

#include "Rectangle.h"
#include "Size.h"
#include <vector>

/**
 * A utility class to help maintain and manage access
 * to a 2-dimensional array.
 */
template<typename T> class Grid final
{
   /** The array representing the 2D grid. */
   std::vector<T> m_grid;

   /** The size of the grid (used for proper 2D grid indexing). */
   geometry::Size m_size;

   public:
      /**
       * Construct an empty, zero-length grid.
       */
      Grid() = default;

      /**
       * Construct a Grid with the specified dimensions.
       *
       * @param size The initial size of the Grid.
       */
      Grid(const geometry::Size& size, const T& defaultValue = T())
      {
         resize(size, defaultValue);
      }

      /**
       * @param x The x-coordinate of the cell to retrieve.
       * @param y The y-coordinate of the cell to retrieve.
       *
       * @return A reference to the contents of the specified Grid cell.
       */
      const T& operator()(unsigned int x, unsigned int y) const
      {
         return m_grid[y * m_size.width + x];
      }

      /**
       * @param x The x-coordinate of the cell to retrieve.
       * @param y The y-coordinate of the cell to retrieve.
       *
       * @return A reference to the contents of the specified Grid cell.
       */
      T& operator()(unsigned int x, unsigned int y)
      {
         return m_grid[y * m_size.width + x];
      }

      /**
       * @return true iff the Grid has no contents.
       */
      bool empty() const noexcept {
         return m_grid.empty();
      }

      /**
       * Clears out the contents of the Grid.
       */
      void clear() noexcept
      {
         m_size = {0,0};
         m_grid.clear();
      }

      /**
       * Resizes the Grid.
       *
       * @param size The new size for the Grid.
       */
      void resize(const geometry::Size& size, const T& defaultValue = T())
      {
         m_size = size;
         m_grid.resize(m_size.getArea(), defaultValue);
      }
   
      /**
       * Sets all grid cells in a given rectangle to a specific value.
       *
       * @param rect The Rectangle representing the subregion to modify in the Grid.
       * @param value The value to fill the subregion with.
       */
      void fillRect(const geometry::Rectangle& rect, const T& value)
      {
         if(!rect.isValid())
         {
            return;
         }

         for(int y = rect.top; y < rect.bottom; ++y)
         {
            for(int x = rect.left; x < rect.right; ++x)
            {
               operator()(x, y) = value;
            }
         }
      }
};

#endif
