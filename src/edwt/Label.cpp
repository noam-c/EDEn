#include "Label.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_EDWT;

namespace edwt
{  Label::Label() : gcn::Label()
   {
   }

   Label::Label(const std::string& caption) : gcn::Label(caption)
   {
   }

   void Label::setForegroundColor(int r, int g, int b, int a)
   {  gcn::Color newColor(r, g, b, a);
      gcn::Widget::setForegroundColor(newColor);
   }
};
