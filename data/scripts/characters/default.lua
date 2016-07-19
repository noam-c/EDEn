function initialize(character)
  print('Initializing character ' .. character:getName{})
  if character:stat{"hp"} == nil then
    print('No HP stat found, setting to max')
    character:stat{"hp", character:stat{"maxHP"}}
  end

  if character:stat{"sp"} == nil then
    print('No SP stat found, setting to max')
    character:stat{"sp", character:stat{"maxSP"}}
  end
end
