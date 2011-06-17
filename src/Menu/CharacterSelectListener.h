#ifndef CHARACTER_SELECT_LISTENER_H
#define CHARACTER_SELECT_LISTENER_H

#include <string>

class CharacterSelectListener
{
   public:
      virtual void characterSelected(const std::string& characterName) = 0;
};

#endif