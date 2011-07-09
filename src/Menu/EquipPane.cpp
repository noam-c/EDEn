#include "EquipPane.h"
#include "PlayerData.h"
#include "Character.h"
#include "Label.h"

EquipPane::EquipPane(Character* character, const gcn::Rectangle& rect) : MenuPane(rect), character(character)
{
   headLabel = new edwt::Label("Head:");
   headEquipLabel = new edwt::Label();

   add(headLabel);
   add(headEquipLabel);
   setCharacter(character);
}

void EquipPane::setCharacter(Character* character)
{
   std::string headEquipName = character->getEquipment().getHeadEquip();
   headEquipLabel->setCaption(headEquipName);
}

EquipPane::~EquipPane()
{
   delete headLabel;
   delete headEquipLabel;
}
