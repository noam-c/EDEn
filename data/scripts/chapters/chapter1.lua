quests:add{name='chapter1'}
kain = roster:createCharacter{'kain'}
kain:addSkill{id=1}
roster:addToParty{kain}
narrate{text="Exodus Draconis", waitForFinish=true}
--narrate{'Our story begins four centuries after the angels left this world.'}
setRegion{'sereia'}
inventory:addItem{id=1,quantity=2}
narrate{"It was a dark and<playSound{'thunder'}>... stormy night..."}
