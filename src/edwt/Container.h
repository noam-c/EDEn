/**
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#ifndef CONTAINER_H
#define CONTAINER_H

#include "guichan.hpp"

namespace edwt
{
   /**
    * Overrides the original Guichan container.
    * This class is currently a stub used strictly for encapsulation purposes.
    *
    * @author Noam Chitayat
    */
   class Container : public gcn::Container
   {
      public:
         /**
          * Constructor.
          */
         Container();
      
         /**
          * @param widget The widget to search for,
          * @returns true iff this container includes the widget.
          */
         bool containsWidget(const gcn::Widget* widget);
   };
};

#endif
