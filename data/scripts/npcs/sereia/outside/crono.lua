function idle(me)
   delay(5000)
   print("Crono: I'm moving after a 5 second wait!")
   move('crono', tilesToPixels(random(1,20)), tilesToPixels(random(1,20)))
   delay(5000)
   print("Crono: I'm moving Marle after a 5 second wait!")
   move('marle', tilesToPixels(random(1,20)), tilesToPixels(random(1,20)))
end