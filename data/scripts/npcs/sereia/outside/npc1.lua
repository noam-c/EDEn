function idle(me)
   delay(1000)
   me:move(map:tilesToPixels(random(0,9)), map:tilesToPixels(random(0,9)))
end

function activate(me)
   lookAtPlayer('npc1')
   say('Oh, hello!', true)
end
