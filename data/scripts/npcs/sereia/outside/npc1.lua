function idle(me)
   delay(1000)
   move('npc1', tilesToPixels(random(0,9)), tilesToPixels(random(0,9)))
end

function activate(me)
   lookAtPlayer('npc1')
   say('Oh, hello!', true)
end
