/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef IMAGE_FORMATTER_H
#define IMAGE_FORMATTER_H

#include <Rocket/Controls/DataFormatter.h>

/**
 * A formatter that converts a data grid text entry into an image tag with
 * the RCSS class <code>datagridimage</code>. Used to render image paths as
 * images in a Rocket GUI.
 *
 * @author Noam Chitayat
 */
class ImageFormatter : public Rocket::Controls::DataFormatter
{
   public:
      /**
       *  Constructor.
       */
      ImageFormatter();

      /**
       * Formats a given set of raw data into an RML image tag.
       *
       * @param formatted_data The string that will hold the final formatted image tag.
       * @param raw_data A single-entry string array containing the path to an image, relative to the RML GUI folder.
       */
      void FormatData(Rocket::Core::String& formatted_data, const Rocket::Core::StringList& raw_data) override;
};

#endif
