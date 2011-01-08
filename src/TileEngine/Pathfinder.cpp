#include "Pathfinder.h"
#include "Map.h"
#include "TileEngine.h"
#include "Point2D.h"
#include <set>
#include <limits>

#include "DebugUtils.h"
const int debugFlag = DEBUG_TILE_ENG;

const int Pathfinder::MOVEMENT_TILE_SIZE = 32;
const float Pathfinder::ROOT_2 = 1.41421356f;
const float Pathfinder::INFINITY = std::numeric_limits<float>::infinity();

Pathfinder::Pathfinder(Map* map) : map(map)
{
   if(map == NULL)
   {
      T_T("Null map encountered when trying to create the collision map.");
   }

   const int collisionTileRatio = TileEngine::TILE_SIZE / MOVEMENT_TILE_SIZE;
   collisionMapWidth = map->getWidth() * collisionTileRatio;
   collisionMapHeight = map->getHeight() * collisionTileRatio;

   bool** passibilityMap = map->getPassibilityMatrix();
   collisionMap = new TileState*[collisionMapHeight];
   for(int y = 0; y < map->getHeight(); ++y)
   {
      TileState* row = collisionMap[y] = new TileState[collisionMapWidth];
      for(int x = 0; x < map->getWidth(); ++x)
      {
         int xOffset = x * collisionTileRatio;
         bool passible = passibilityMap[y][x];
         for(int i = 0; i < collisionTileRatio; ++i)
         {
            row[i + xOffset] = passible ? FREE : OBSTACLE;
         }
      }
   }

   initRoyFloydWarshallMatrices();
}

Point2D Pathfinder::tileNumToCoords(int tileNum)
{
   return Point2D(tileNum / collisionMapWidth, tileNum % collisionMapWidth);
}

Point2D Pathfinder::tileNumToPixels(int tileNum)
{
   Point2D p = tileNumToCoords(tileNum);
   p.x *= TileEngine::TILE_SIZE;
   p.y *= TileEngine::TILE_SIZE;
   return p;
}

int Pathfinder::coordsToTileNum(int x, int y)
{
   return (y * collisionMapWidth + x);
}

int Pathfinder::pixelsToTileNum(int x, int y)
{
   return (y * collisionMapWidth + x) / TileEngine::TILE_SIZE;
}

void Pathfinder::initRoyFloydWarshallMatrices()
{
   const int numTiles = collisionMapWidth*collisionMapHeight;
   
   distanceMatrix = new float*[numTiles];
   successorMatrix = new int*[numTiles];

   for(int i = 0; i < numTiles; ++i)
   {
      distanceMatrix[i] = new float[numTiles];
      successorMatrix[i] = new int[numTiles];
   }

   Point2D aTile;
   Point2D bTile;

   for(int a = 0; a < numTiles; ++a)
   {
      aTile = tileNumToCoords(a);
      for(int b = 0; b < numTiles; ++b)
      {
         if(a == b)
         {
            distanceMatrix[a][b] = 0;
            successorMatrix[a][b] = -1;
         }
         else
         {
            bTile = tileNumToCoords(b);
            if(collisionMap[aTile.y][aTile.x] == OBSTACLE || collisionMap[bTile.y][bTile.x] == OBSTACLE)
            {
               distanceMatrix[a][b] = INFINITY;
               successorMatrix[a][b] = -1;
            }
            else
            {
               bool xAdjacent = aTile.x - 1 <= bTile.x && bTile.x <= aTile.x + 1;
               bool yAdjacent = aTile.y - 1 <= bTile.y && bTile.y <= aTile.y + 1;

               if(!xAdjacent || !yAdjacent)
               {
                  distanceMatrix[a][b] = INFINITY;
                  successorMatrix[a][b] = -1;
               }
               else
               {
                  successorMatrix[a][b] = b;
                  if(aTile.x != bTile.x && aTile.y != bTile.y)
                  {
                     distanceMatrix[a][b] = ROOT_2;
                  }
                  else
                  {
                     distanceMatrix[a][b] = 1;
                  }
               }
            }
         }
      }
   }

   float distance;
   for(int i = 0; i < numTiles; ++i)
   {
      for(int a = 0; a < numTiles; ++a)
      {
         for(int b = 0; b < numTiles; ++b)
         {
            distance = distanceMatrix[a][i] + distanceMatrix[i][b];
            if(distance < distanceMatrix[a][b])
            {
               distanceMatrix[a][b] = distance;
               successorMatrix[a][b] = successorMatrix[a][i];
            }
         }
      }
   }
}

std::queue<Point2D*> Pathfinder::findBestPath(int srcX, int srcY, int dstX, int dstY, PathfindingStyle style)
{
   switch(style)
   {
      case RFW:
      {
         return findRFWPath(srcX, srcY, dstX, dstY);
      }
      case STRAIGHT:
      default:
      {
         return getStraightPath(srcX, srcY, dstX, dstY);
      }
   }
}

std::queue<Point2D*> Pathfinder::getStraightPath(int srcX, int srcY, int dstX, int dstY)
{
   std::queue<Point2D*> path;
   while(srcX > dstX + MOVEMENT_TILE_SIZE)
   {
      srcX -= MOVEMENT_TILE_SIZE;
      path.push(new Point2D(srcX, srcY));
   }

   while(srcX < dstX - MOVEMENT_TILE_SIZE)
   {
      srcX += MOVEMENT_TILE_SIZE;
      path.push(new Point2D(srcX, srcY));
   }

   while(srcY > dstY + MOVEMENT_TILE_SIZE)
   {
      srcY -= MOVEMENT_TILE_SIZE;
      path.push(new Point2D(srcX, srcY));
   }

   while(srcY < dstY - MOVEMENT_TILE_SIZE)
   {
      srcY += MOVEMENT_TILE_SIZE;
      path.push(new Point2D(srcX, srcY));
   }

   return path;
}

std::queue<Point2D*> Pathfinder::findRFWPath(int srcX, int srcY, int dstX, int dstY)
{
   std::queue<Point2D*> path;

   int srcTileNum = pixelsToTileNum(srcX, srcY);
   const int dstTileNum = pixelsToTileNum(dstX, dstY);

   for(;;)
   {
      int nextTile = successorMatrix[srcTileNum][dstTileNum];
      if(nextTile == -1)
      {
         break;
      }
      
      path.push(new Point2D(tileNumToPixels(nextTile)));
      srcTileNum = nextTile;
   }

   return path;
}

bool Pathfinder::isWalkable(int x, int y)
{
   if(x < 0 || y < 0) return false;
   if(x >= collisionMapWidth) return false;
   if(y >= collisionMapHeight) return false;

   return collisionMap[x][y] == FREE;
}

bool Pathfinder::occupyPoint(int x, int y, int width, int height)
{
   int collisionMapLeft = x/MOVEMENT_TILE_SIZE;
   int collisionMapRight = (x + width)/MOVEMENT_TILE_SIZE;
   int collisionMapTop = y/MOVEMENT_TILE_SIZE;
   int collisionMapBottom = (y + height)/MOVEMENT_TILE_SIZE;

   for(int collisionMapX = collisionMapLeft; collisionMapX <= collisionMapRight; ++collisionMapX)
   {
      for(int collisionMapY = collisionMapTop; collisionMapY <= collisionMapBottom; ++collisionMapY)
      {
         if(!collisionMap[collisionMapY][collisionMapX] != FREE) return false;
      }
   }

   for(int collisionMapX = collisionMapLeft; collisionMapX <= collisionMapRight; ++collisionMapX)
   {
      for(int collisionMapY = collisionMapTop; collisionMapY <= collisionMapBottom; ++collisionMapY)
      {
         collisionMap[collisionMapY][collisionMapX] = CHARACTER;
      }
   }

   return true;
}

void Pathfinder::freePoint(int x, int y, int width, int height)
{
   int collisionMapLeft = x/MOVEMENT_TILE_SIZE;
   int collisionMapRight = (x + width)/MOVEMENT_TILE_SIZE;
   int collisionMapTop = y/MOVEMENT_TILE_SIZE;
   int collisionMapBottom = (y + height)/MOVEMENT_TILE_SIZE;

   for(int collisionMapX = collisionMapLeft; collisionMapX <= collisionMapRight; ++collisionMapX)
   {
      for(int collisionMapY = collisionMapTop; collisionMapY <= collisionMapBottom; ++collisionMapY)
      {
         collisionMap[collisionMapY][collisionMapX] = FREE;
      }
   }
}

void Pathfinder::deleteRoyFloydWarshallMatrices()
{
   const int numTiles = collisionMapWidth * collisionMapHeight;
   if(distanceMatrix)
   {
      for(int i = 0; i < numTiles; ++i)
      {
         delete [] distanceMatrix[i];
      }

      delete [] distanceMatrix;
      distanceMatrix = NULL;
   }

   if(successorMatrix)
   {
      for(int i = 0; i < numTiles; ++i)
      {
         delete [] successorMatrix[i];
      }

      delete [] successorMatrix;
      successorMatrix = NULL;
   }}

void Pathfinder::deleteCollisionMap()
{
   if(collisionMap)
   {
      for(int i = 0; i < collisionMapHeight; ++i)
      {
         delete [] collisionMap[i];
      }

      delete [] collisionMap;
      collisionMap = NULL;
   }
}

Pathfinder::~Pathfinder()
{
   deleteRoyFloydWarshallMatrices();
   deleteCollisionMap();
}
