function idle(me)
   me:lookAt{playerSprite}
end

function activate(me)
   me:lookAt{playerSprite}
   say{'Wot? Wanna fight?', waitForFinish=true}
   startBattle{}
   say{'Is that it?', waitForFinish=true}
end
