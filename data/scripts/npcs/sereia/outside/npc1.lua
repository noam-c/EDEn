function idle(me)
   delay(1000)
   me:move(map:tilesToPixels(random(0,9)), map:tilesToPixels(random(0,9)))
end

function activate(me)
   me:lookAt(playerSprite)
   if not quests:get('chapter1/carryAConvo'):isStarted() then
      say('Oh, hello!')
      say('Can you do me a favour? Can you talk to the other guy for me?', true)
      quests:get('chapter1'):add('carryAConvo', 'Carry a conversation between two NPCs'):add('talkToNPC', 'Talk to the second NPC')
   elseif quests:get('chapter1/carryAConvo'):isComplete() then
      say('Thank you again!', true)
   elseif quests:get('chapter1/carryAConvo/talkToNPC'):isComplete() then
      say('Thank you so much! I\'d reward you, but that hasn\'t been implemented yet!', true)
      quests:get('chapter1/carryAConvo'):complete()
   else
      say('So did you talk to the other guy yet?', true)
   end
end
