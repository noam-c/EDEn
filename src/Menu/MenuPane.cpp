#include "MenuPane.h"

MenuPane::MenuPane()
{
}

void MenuPane::draw(gcn::Graphics* graphics)
{
   if (isOpaque())
   {
      graphics->setColor(getBaseColor());
      graphics->fillRectangle(gcn::Rectangle(0, 0, getWidth(), getHeight()));
   }

   drawChildren(graphics);
}

MenuPane::MenuPane(const gcn::Rectangle& dimension)
{
   setDimension(dimension);
   setOpaque(false);
}