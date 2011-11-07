#include "MenuPane.h"

MenuPane::MenuPane()
{
}

MenuPane::MenuPane(const gcn::Rectangle& dimension)
{
   setDimension(dimension);
   setOpaque(false);
}

void MenuPane::logic()
{
   edwt::Container::logic();
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

MenuPane::~MenuPane()
{
}