/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#ifndef ENUM_UTILS_H
#define ENUM_UTILS_H

#include <type_traits>

class EnumUtils
{
   public:
      template<class Enum, typename std::enable_if<std::is_enum<Enum>::value, bool>::type = true> static constexpr typename std::underlying_type<Enum>::type toNumber(Enum value)
         {
            return static_cast<typename std::underlying_type<Enum>::type>(value);
         }
};

#endif