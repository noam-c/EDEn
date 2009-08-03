function idle(me)
   delay(10000)
   print("Crono: I'm idling after a 10 second wait!")
   me:move(x,y)
   marle = getNPC('marle')
   marle:move(x2,y2)
end