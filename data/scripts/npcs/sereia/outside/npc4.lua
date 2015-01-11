function activate(me)
   me:lookAt{playerSprite}
   saveChoice = say{'Would you like to save your game?', choices={"Save", "Don't Save"}}
   if saveChoice == 0 then
      map:openSaveMenu{}
   end

   say{'See you later!'}
end
