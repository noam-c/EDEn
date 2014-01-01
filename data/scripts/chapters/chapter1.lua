quests:add{name='chapter1'}
kain = roster:createCharacter{'kain'}
roster:addToParty{kain}
narrate{"EDEn Chapter 1", waitForFinish=true}
setRegion{'sereia'}
inventory:addItem{id=1,quantity=2}
narrate{"It was a dark and<playSound{'thunder'}>... stormy night..."}
playerSprite:show{x=map:tilesToPixels{0}, y=map:tilesToPixels{6}}
map:slideCamera{destinationX=map:tilesToPixels{29}, destinationY=map:tilesToPixels{6}}
map:slideCamera{destinationX=map:tilesToPixels{0}, destinationY=map:tilesToPixels{6}}
map:lockCameraToTarget{playerSprite}
