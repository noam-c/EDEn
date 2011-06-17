#include "StatusMenu.h"
#include "StatusPane.h"

StatusMenu::StatusMenu(edwt::Container* top, edwt::Container& menuPane, Character& character) : MenuState(top, menuPane), character(character)
{
   statusPane = new StatusPane(character, menuPane.getDimension());
   menuPane.add(statusPane);
}

StatusMenu::~StatusMenu()
{
   menuPane.remove(statusPane);
   delete statusPane;
}