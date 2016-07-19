/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "JsonUtils.h"

namespace JsonUtils
{
   template<> std::string convertValue(const Json::Value& jsonValue)
   {
      return jsonValue.asString();
   }
   
   template<> unsigned int convertValue(const Json::Value& jsonValue)
   {
      return jsonValue.asUInt();
   }

   template<> int convertValue(const Json::Value& jsonValue)
   {
      return jsonValue.asInt();
   }
};
