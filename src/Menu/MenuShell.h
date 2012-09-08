#ifndef MENU_SHELL_H
#define MENU_SHELL_H

namespace Rocket
{
   namespace Core
   {
      class Context;
      class ElementDocument;
   };
};

class MenuShell
{
      Rocket::Core::Context* rocketContext;
      Rocket::Core::ElementDocument* shellDocument;

      public:
         MenuShell();
         MenuShell(const MenuShell& menuShell);
         ~MenuShell();

         Rocket::Core::Context* getContext() const;
         Rocket::Core::ElementDocument* getShellDocument() const;
};

#endif
