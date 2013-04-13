function activate(me)
   me:lookAt{playerSprite}
   say{'I\'m the music man!', waitForFinish=true}
   if not isMusicPlaying() then
      say{'I\'m going to turn on some music.', waitForFinish=true}
      playMusic{'town.mid'}
   else
      say{'I\'m going to fade out the music.', waitForFinish=true}
      stopMusic{fadeTime=1000}
   end
end
