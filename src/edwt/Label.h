/**
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#ifndef LABEL_H
#define LABEL_H

#include "guichan.hpp"

namespace edwt
{
   /**
    * Overrides the original Guichan Label.
    * This class is currently a stub for encapsulation purposes.
    *
    * @author Noam Chitayat
    */
   class Label : public gcn::Label
   {
      public:
         /**
          * Constructor.
          */
         Label();

         /**
          * Constructor.
          *
          * @param caption A caption to set for this label.
          */
         Label(const std::string& caption);
   };
};

#endif
