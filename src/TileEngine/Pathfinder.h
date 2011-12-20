/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <list>
#include <vector>

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
      A_STAR,
   };

   enum OccupantType
   {
      FREE,
      CHARACTER,
      OBSTACLE,
   };

   struct TileState
   {
      OccupantType occupantType;
      void* occupant;
      
      TileState() : occupantType(FREE), occupant(NULL) {}
      TileState(OccupantType type) : occupantType(type), occupant(NULL) {}
      TileState(OccupantType type, void* occupant) : occupantType(type), occupant(occupant) {}
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

   void deleteRoyFloydWarshallMatrices();
   void deleteCollisionMap();

   bool occupyPoint(int x, int y, int width, int height, TileState state);
   void freePoint(int x, int y, int width, int height);

   bool addObstacle(int x, int y, int width, int height);
   void setPoint(int left, int top, int right, int bottom, TileState state);

   public:
      typedef std::list<Point2D> Path;

      Pathfinder(Map* newMap, std::vector<Obstacle*> obstacles);

      Path findPath(int srcX, int srcY, int dstX, int dstY, PathfindingStyle = A_STAR);

      bool beginMovement(NPC* npc, int srcX, int srcY, int dstX, int dstY, int width, int height);
      void endMovement(int srcX, int srcY, int dstX, int dstY, int width, int height);

      void draw();

      ~Pathfinder();

   private:
      Path getStraightPath(int srcX, int srcY, int dstX, int dstY);
      Path findRFWPath(int srcX, int srcY, int dstX, int dstY);
      Path findAStarPath(int srcX, int srcY, int dstX, int dstY);

      class AStarPoint;
      void evaluateAdjacentNodes(const std::vector<Point2D>& adjacentNodes, const AStarPoint* cheapestPoint, float traversalCost, int destinationTileNum, std::vector<AStarPoint*>& openSet, std::vector<bool>& discovered);
};

#endif
