#include "Label.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_EDWT;

namespace edwt
{
   Label::Label() : gcn::Label()
   {
   }

   Label::Label(const std::string& caption) : gcn::Label(caption)
   {
   }
};
