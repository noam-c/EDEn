map:addNPC{'npc1', spritesheet='npc1', x=map:tilesToPixels{0}, y=map:tilesToPixels{0}}
map:addNPC{'npc2', spritesheet='npc1', x=map:tilesToPixels{0}, y=map:tilesToPixels{3}}
map:addNPC{'npc3', spritesheet='npc1', x=map:tilesToPixels{0}, y=map:tilesToPixels{9}, direction=Direction.Right}
map:addNPC{'npc4', spritesheet='npc1', x=map:tilesToPixels{17}, y=map:tilesToPixels{3}}
--map:addNPC{'npc5', spritesheet='npc1', x=map:tilesToPixels{4}, y=map:tilesToPixels{4}}
--map:addNPC{'npc6', spritesheet='npc1', x=map:tilesToPixels{4}, y=map:tilesToPixels{9}}
--map:addNPC{'npc7', spritesheet='npc1', x=map:tilesToPixels{9}, y=map:tilesToPixels{0}}
--map:addNPC{'npc8', spritesheet='npc1', x=map:tilesToPixels{9}, y=map:tilesToPixels{4}}
--map:addNPC{'npc9', spritesheet='npc1', x=map:tilesToPixels{9}, y=map:tilesToPixels{9}}

niceGarden = 'Nice garden.'
function gardenEntered()
   say{niceGarden}
end

map:addTriggerListener{name='garden', handler=gardenEntered}
