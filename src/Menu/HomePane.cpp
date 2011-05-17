#include "HomePane.h"
#include "PlayerData.h"

HomePane::HomePane(PlayerData& playerData, const gcn::Rectangle& rect) : MenuPane(rect), playerData(playerData)
{
}

HomePane::~HomePane()
{
}