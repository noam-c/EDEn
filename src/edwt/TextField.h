/**
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#ifndef TEXT_FIELD_H
#define TEXT_FIELD_H

#include "guichan.hpp"

namespace edwt
{
   /**
    * Overrides the original Guichan TextField.
    *
    * @author Noam Chitayat
    */
   class TextField : public gcn::TextField
   {
      public:
         /**
          * Constructor.
          */
         TextField();

         /**
          * Constructor.
          *
          * @param text Text to set for this field.
          */
         TextField(const std::string& text);
   };
};

#endif
