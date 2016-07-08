for i, chara in ipairs(roster:getParty{}) do
  battle:addCombatant{
    id=chara:getId(),
    spritesheet=chara:getSpritesheetId(),
    x=i*50, y=i*100,
    direction=Direction.Right
  }
end
