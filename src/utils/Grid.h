/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#ifndef GRID_H
#define GRID_H

#include "Size.h"
#include <vector>

/**
 * A utility class to help maintain and manage access
 * to a 2-dimensional array.
 */
template<typename T> class Grid
{
   /** The array representing the 2D grid. */
   std::vector<T> m_grid;

   /** The width of the grid (used for proper 2D grid indexing). */
   unsigned int m_width;

   public:
      /**
       * Construct an empty, zero-length grid.
       *
       * @param size The initial size of the Grid.
       */
      Grid() :
         m_width(0)
      {
      }

      /**
       * Construct a Grid with the specified dimensions.
       *
       * @param size The initial size of the Grid.
       */
      Grid(const shapes::Size& size, const T& defaultValue = T())
      {
         resize(size, defaultValue);
      }

      /**
       * Copy constructor.
       */
      Grid(const Grid<T>& other) = default;

      /**
       * Destructor.
       */
      ~Grid() = default;

      /**
       * @param x The x-coordinate of the cell to retrieve.
       * @param y The y-coordinate of the cell to retrieve.
       *
       * @return A reference to the contents of the specified Grid cell.
       */
      const T& operator()(unsigned int x, unsigned int y) const
      {
         return m_grid[y * m_width + x];
      }

      /**
       * @param x The x-coordinate of the cell to retrieve.
       * @param y The y-coordinate of the cell to retrieve.
       *
       * @return A reference to the contents of the specified Grid cell.
       */
      T& operator()(unsigned int x, unsigned int y)
      {
         return m_grid[y * m_width + x];
      }

      /**
       * @return true iff the Grid has no contents.
       */
      bool empty() const {
         return m_grid.empty();
      }

      /**
       * Clears out the contents of the Grid.
       */
      void clear()
      {
         m_width = 0;
         m_grid.clear();
      }

      /**
       * Resizes the Grid.
       *
       * @param size The new size for the Grid.
       */
      void resize(const shapes::Size& size, const T& defaultValue = T())
      {
         m_width = size.width;
         m_grid.resize(size.width * size.height, defaultValue);
      }
};

#endif
