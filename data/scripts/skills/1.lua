function canUse()
   return true;
end

function onMenuUse(character)
   print(character:getName() .. ' is casting Heal in the menu.')
end

function onFieldUse(character)
   say(character:getName() .. ' is casting Heal in the field.')
end

function onBattleUse(character)
   print(character:getName() .. ' is casting Heal in battle.')
end
