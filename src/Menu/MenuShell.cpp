#include "MenuShell.h"
#include "GraphicsUtil.h"
#include <Rocket/Core.h>

MenuShell::MenuShell()
{
   rocketContext = GraphicsUtil::getInstance()->createRocketContext("menu");
   shellDocument = rocketContext->LoadDocument("data/gui/menushell.rml");

   if(shellDocument != NULL)
   {
      shellDocument->Show();
   }
}

MenuShell::MenuShell(const MenuShell& menuShell)
   : rocketContext(menuShell.rocketContext), shellDocument(menuShell.shellDocument)
{
   rocketContext->AddReference();
   shellDocument->AddReference();
}

MenuShell::~MenuShell()
{
   shellDocument->RemoveReference();
   rocketContext->RemoveReference();
}

Rocket::Core::Context* MenuShell::getContext() const
{
   return rocketContext;
}

Rocket::Core::ElementDocument* MenuShell::getShellDocument() const
{
   return shellDocument;
}
