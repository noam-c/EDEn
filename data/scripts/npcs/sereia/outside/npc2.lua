function idle(me)
   delay{1000}
   me:move{map:tilesToPixels{random{0,9}}, map:tilesToPixels{random{0,9}}}
end

function activate(me)
   if quests:isComplete{'chapter1/carryAConvo'} then
      say{'Can\'t talk, I\'m busy.'}
   else
      me:lookAt{playerSprite}
      say{'Yeah, what?', waitForFinish=true}
   
      if quests:isStarted{'chapter1/carryAConvo'} then
         say{'The other guy wanted to say hi?', waitForFinish=true}
         say{'Weird. Alright, say hi back.', waitForFinish=true}
         quests:get{'chapter1/carryAConvo'}:add{'talkToNPC'}:complete()
      end
   end
end
