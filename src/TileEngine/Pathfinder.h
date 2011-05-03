#ifndef __PATHFINDER_H_
#define __PATHFINDER_H_

#include <queue>

class Obstacle;
class Map;
class NPC;
struct Point2D;

class Pathfinder
{
   enum PathfindingStyle
   {
      RFW,
      STRAIGHT,
      //A_STAR,
   };

   enum TileState
   {
      FREE,
      CHARACTER,
      OBSTACLE,
   };

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

   bool occupyPoint(int x, int y, int width, int height, TileState state);
   bool addObstacle(int x, int y, int width, int height);
   void setPoint(int left, int top, int right, int bottom, TileState state);

   public:
      Pathfinder(Map* newMap, std::vector<Obstacle*> obstacles);

      std::queue<Point2D*> findBestPath(int srcX, int srcY, int dstX, int dstY, PathfindingStyle = RFW);

      bool occupyPoint(int x, int y, int width, int height);
      void freePoint(int x, int y, int width, int height);

      ~Pathfinder();
};

#endif