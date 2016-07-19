/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include <string>

#include "json.h"

namespace JsonUtils
{
   /**
    * Converts a JsonCpp Json::Value object into a built-in type.
    * Valid conversions are defined in JsonUtils.cpp as specializations.
    *
    * WARNING: This function is not generic since there exist only
    * a small set of valid conversions from Json::Value objects.
    * Attempts to convert to an unsupported type will lead to a linker error.
    *
    * @return The converted form of jsonValue.
    */
   template<typename V> V convertValue(const Json::Value& jsonValue);

   template<typename V> std::map<std::string, V> toStlMap(const Json::Value& obj)
   {
      std::map<std::string, V> newMap;

      for(auto iter = obj.begin(); iter != obj.end(); ++iter)
      {
         newMap.emplace(std::piecewise_construct,
                        std::forward_as_tuple(iter.key().asString()),
                        std::forward_as_tuple(JsonUtils::convertValue<V>(*iter)));
      }

      return newMap;
   }
};

#endif
