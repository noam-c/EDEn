function canUse()
   return true;
end

function onMenuUse()
   print('Casting Heal in the menu.')
end

function onFieldUse()
   say('Casting Heal in the field.')
end

function onBattleUse()
   print('Casting Heal in battle.')
end
