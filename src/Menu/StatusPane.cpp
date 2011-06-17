#include "StatusPane.h"
#include "Character.h"
#include "CharacterModule.h"

StatusPane::StatusPane(Character& character, const gcn::Rectangle& rect) : MenuPane(rect), character(character)
{
   module = new CharacterModule();
   module->setCharacter(&character);
   add(module);
}

StatusPane::~StatusPane()
{
   delete module;
}