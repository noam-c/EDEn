#ifndef __ICON_H_
#define __ICON_H_

#include <string>
#include "guichan.hpp"

namespace edwt
{
   /**
    * Overrides the original Guichan Label.
    * This class is currently a stub for encapsulation purposes.
    *
    * @author Noam Chitayat
    */
   class Icon : public gcn::Icon
   {
      public:
        /**
         * Default constructor.
         */
        Icon();

        /**
         * Constructor.
         *
         * @param filename The filename of the image to display.
         */
        Icon(const std::string& filename);

        /**
         * Constructor.
         *
         * @param image The image to display.
         */
        Icon(const gcn::Image* image);

        /**
         * Sets the image to display. Existing image is freed automatically
         * if it was loaded internally.
         *
         * @param filename The image to load for display.
         */
        void setImage(const std::string& filename);

        /**
         * Clears the image loaded by the Icon. Existing image is freed automatically
         * if it was loaded internally.
         */
        void clearImage();
   };
};

#endif
