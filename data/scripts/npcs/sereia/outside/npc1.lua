function idle(me)
   delay{1000}
   me:move{map:tilesToPixels{random{0,9}}, map:tilesToPixels{random{0,9}}}
end

function activate(me)
   me:lookAt{playerSprite}
   if not quests:get{path='chapter1/carryAConvo'}:isStarted() then
      say{'Oh, hello!'}
      say{'Can you do me a favour? Can you talk to the other guy for me?', waitForFinish=true}
      local carryAConvoQuest = quests:get{'chapter1'}:add{id='carryAConvo', description='Carry a conversation between two NPCs'}
      carryAConvoQuest:add{id='talkToNPC', description='Talk to the second NPC'}
   elseif quests:get{'chapter1/carryAConvo'}:isComplete() then
      say{'Thank you again!', waitForFinish=true}
   elseif quests:get{'chapter1/carryAConvo/talkToNPC'}:isComplete() then
      say{'Thank you so much! Have a reward!', waitForFinish=true}
      inventory:addItem{id=1, quantity=1}
      quests:get{'chapter1/carryAConvo'}:complete()
   else
      say{'So did you talk to the other guy yet?', waitForFinish=true}
   end
end
