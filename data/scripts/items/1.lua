function canUse()
   return true;
end

function onMenuUse()
   print('Used a potion in the menu.')
end

function onFieldUse()
   say('Used a potion in the field.')
end

function onBattleUse()
   print('Used a potion in battle.')
end
