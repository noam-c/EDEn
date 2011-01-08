#ifndef __PATHFINDER_H_
#define __PATHFINDER_H_

#include <queue>

class Map;
class NPC;
struct Point2D;

enum TileState
{
   FREE,
   CHARACTER,
   OBSTACLE,
};

enum PathfindingStyle
{
   RFW,
   STRAIGHT,
   //A_STAR,
};

class Pathfinder
{
   static const int MOVEMENT_TILE_SIZE;
   static const float ROOT_2;
   static const float INFINITY;

   Map* map;

   int collisionMapWidth;
   int collisionMapHeight;
   TileState** collisionMap;

   float** distanceMatrix;
   int** successorMatrix;

   inline Point2D tileNumToPixels(int tileNum);
   inline int pixelsToTileNum(int x, int y);

   inline Point2D tileNumToCoords(int tileNum);
   inline int coordsToTileNum(int x, int y);

   void initRoyFloydWarshallMatrices();
   bool isWalkable(int x, int y);
   std::queue<Point2D*> findRFWPath(int srcX, int srcY, int dstX, int dstY);
   std::queue<Point2D*> getStraightPath(int srcX, int srcY, int dstX, int dstY);

   void deleteRoyFloydWarshallMatrices();
   void deleteCollisionMap();

   public:
      Pathfinder(Map* newMap);

      std::queue<Point2D*> findBestPath(int srcX, int srcY, int dstX, int dstY, PathfindingStyle = RFW);

      bool occupyPoint(int x, int y, int width, int height);
      void freePoint(int x, int y, int width, int height);

      ~Pathfinder();
};

#endif