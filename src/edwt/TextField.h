#ifndef __TEXT_FIELD_H_
#define __TEXT_FIELD_H_

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
