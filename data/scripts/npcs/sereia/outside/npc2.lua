function idle(me)
   delay(1000)
   me:move(map:tilesToPixels(random(0,9)), map:tilesToPixels(random(0,9)))
end

function activate(me)
   if quests:get('chapter1/carryAConvo'):isComplete() then
      say('Can\'t talk, I\'m busy.')
   else
      me:lookAt(playerSprite)
      say('Yeah, what?', true)
   
      if quests:get('chapter1/carryAConvo'):isStarted() then
         say('The other guy wanted to say hi?', true)
         say('Weird. Alright, say hi back.', true)
         quests:get('chapter1/carryAConvo/talkToNPC'):complete()
      end
   end
end
