#ifndef __PLAYER_CHARACTER_H_
#define __PLAYER_CHARACTER_H_

#include <string>

class Spritesheet;
class Sprite;

class PlayerCharacter
{
   static const int WALKING_SPEED = 1;
   static const std::string WALKING_PREFIX;
   static const std::string STANDING_PREFIX;

   enum MovementDirection
   {
      UP,
      DOWN,
      LEFT,
      RIGHT,
      NUM_DIRECTIONS
   };

   int playerX, playerY;
   int xSpeed, ySpeed;
   MovementDirection currDirection;

   Sprite* sprite;

   public:
      PlayerCharacter(Spritesheet* sheet, int x, int y);
      void step(long timePassed);
      void draw();
      ~PlayerCharacter();
};

#endif
