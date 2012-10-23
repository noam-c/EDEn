/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef MENU_SHELL_H
#define MENU_SHELL_H

#include <vector>
#include <string>
#include "EdenRocketBindings.h"

namespace Rocket
{
   namespace Core
   {
      class Context;
      class ElementDocument;
      class Element;
      class Event;
   };
};

class MenuState;

class MenuShell
{
      Rocket::Core::Context* rocketContext;
      Rocket::Core::ElementDocument* shellDocument;
      Rocket::Core::Element* sidebarElement;

      /** The event binding collection for the sidebar */
      EdenRocketBindings<MenuShell> bindings;

      MenuState* currentState;

      void refresh();

      public:
         MenuShell();
         ~MenuShell();

         Rocket::Core::Context* getContext() const;
         Rocket::Core::ElementDocument* getShellDocument() const;
         void changeMenuState(MenuState* newState);
         void sidebarClicked(Rocket::Core::Event* event);
};

#endif
