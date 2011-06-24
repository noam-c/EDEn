#include "StatusPane.h"
#include "Character.h"
#include "CharacterModule.h"

StatusPane::StatusPane(Character* character, const gcn::Rectangle& rect) : MenuPane(rect), character(character)
{
   module = new CharacterModule();
   module->setCharacter(character);
   add(module);
}

void StatusPane::setCharacter(Character* character)
{
   module->setCharacter(character);
}

StatusPane::~StatusPane()
{
   delete module;
}