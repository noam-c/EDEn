#ifndef __LABEL_H_
#define __LABEL_H_

#include "guichan.hpp"

namespace edwt
{   /**
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

          /**
           * Sets the foreground color of the label
           * \todo This method can definitely be used as a generic edwt utility.
           *
           * @param r Red color component (range 0-255).
	        * @param g Green color component (range 0-255).
	        * @param b Blue color component (range 0-255).
	        * @param a Color alpha, used for transparency. A value of 0 means totaly transparent, 255 is totally opaque (the default). 
           */
          void setForegroundColor(int r, int g, int b, int a = 255); 
    };
};

#endif
