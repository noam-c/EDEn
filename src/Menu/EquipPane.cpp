#include "EquipPane.h"
#include "PlayerData.h"

EquipPane::EquipPane(PlayerData& playerData, const gcn::Rectangle& rect) : MenuPane(rect), playerData(playerData)
{
}

EquipPane::~EquipPane()
{
}