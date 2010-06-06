#include "TextField.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_EDWT;

namespace edwt
{
   TextField::TextField() : gcn::TextField()
   {
      adjustSize();
   }

   TextField::TextField(const std::string& text) : gcn::TextField(text)
   {
   }
};
