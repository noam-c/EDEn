function activate(me)
   me:lookAt{playerSprite}
   say{'You can save your game now.', waitForFinish=true}
   map:openSaveMenu{}
end
