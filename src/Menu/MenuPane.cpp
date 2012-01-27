/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

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
