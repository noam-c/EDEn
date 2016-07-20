/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include <sstream>

#include "Direction.h"

namespace geometry
{
   Direction toDirection(const std::string& name)
   {
      std::ostringstream os;
      
      /*
       I do this manually because the STL's way of
       lowercasing is difficult to read/maintain
       when you're only working on chars. Also, std::string
       character replacement is awful. You can't
       even supply a lambda to select the
       replacement character. I can't believe I'm
       saying this, but JS does it better. :/
      */
      for(auto c : name)
      {
         if(c >= 'A' && c <= 'Z')
         {
            os << (c - 'A' + 'a');
         }
         else if(c >= 'a' && c <= 'z')
         {
            os << c;
         }
      }

      const auto lower = os.str();
      if(lower == "up")
      {
         return Direction::UP;
      }
      else if(lower == "down")
      {
         return Direction::DOWN;
      }
      else if(lower == "left")
      {
         return Direction::LEFT;
      }
      else if(lower == "right")
      {
         return Direction::RIGHT;
      }
      else if(lower == "upleft")
      {
         return Direction::UP_LEFT;
      }
      else if(lower == "upright")
      {
         return Direction::UP_RIGHT;
      }
      else if(lower == "downleft")
      {
         return Direction::DOWN_LEFT;
      }
      else if(lower == "downright")
      {
         return Direction::DOWN_RIGHT;
      }
      
      return Direction::NONE;
   }
};