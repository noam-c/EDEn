/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <list>
#include <vector>

class Obstacle;
class Map;
class NPC;
struct Point2D;

/**
 * The Pathfinder class binds to a Map and stores the locations of entities.
 * In doing so, it applies pathfinding algorithms to dynamically compute best paths around entities on the map.
 * Pathfinder instances provide an interface to entities like the NPC and PlayerCharacter to detect collisions and route around them.
 *
 * @author Noam Chitayat
 */
class Pathfinder
{
   /**
    * The kinds of entities that can occupy a tile.
    */
   enum OccupantType
   {
      /** No entity is on the tile */
      FREE,

      /** A character is on the tile */
      CHARACTER,

      /** An obstacle (or impassable terrain) is on the tile */
      OBSTACLE,
   };

   /**
    * The state of a tile, specifically the kind of entity occupying it and
    * a pointer to that entity.
    */
   struct TileState
   {
      /** The type of occupant in the tile. */
      OccupantType occupantType;

      /** The occupant in the tile. */
      void* occupant;

      /**
       * Default constructor.
       */
      TileState() : occupantType(FREE), occupant(NULL) {}

      /**
       * Constructor.
       *
       * @param type The type of entity occupying the tile.
       */
      TileState(OccupantType type) : occupantType(type), occupant(NULL) {}

      /**
       * Constructor.
       *
       * @param type The type of entity occupying the tile.
       * @param occupant The occupant in the tile.
       */
      TileState(OccupantType type, void* occupant) : occupantType(type), occupant(occupant) {}
   };

   /** The size of a movement tile (used to control pathfinding granularity) */
   static const int MOVEMENT_TILE_SIZE;

   /** The square root of 2. */
   static const float ROOT_2;

   /** Floating-point notation for infinity. */
   static const float INFINITY;

   /** The map that the pathfinder is tracking. */
   const Map* map;

   /** The width of the pathfinder map. */
   int collisionMapWidth;

   /** The height of the pathfinder map. */
   int collisionMapHeight;

   /** The map of entities and states for each of the tiles. */
   TileState** collisionMap;

   /** The Roy-Floyd-Warshall distance matrix. This 2D array holds best-path distances between all tiles. */
   float** distanceMatrix;

   /** The Roy-Floyd-Warshall successor matrix. This 2D array holds the best tile to move to, given a source and a destination. */
   int** successorMatrix;

   /**
    * Convert a tile number into pixel coordinates.
    *
    * @param The tile number when counting the tiles from left to right, then top to bottom.
    */
   inline Point2D tileNumToPixels(int tileNum);

   /**
    * Convert pixel coordinates into a tile number.
    *
    * @param pixelLocation The coordinates of the location (in pixels)
    */
   inline int pixelsToTileNum(Point2D pixelLocation);

   /**
    * Convert a tile number into tile coordinates.
    *
    * @param The tile number when counting the tiles from left to right, then top to bottom.
    */
   inline Point2D tileNumToCoords(int tileNum);

   /**
    * Convert tile coordinates into a tile number.
    *
    * @param tileLocation The coordinates of the location (in tiles)
    */
   inline int coordsToTileNum(Point2D tileLocation);
   
   /** 
    * Runs the Roy-Floyd-Warshall algorithm on the map
    * to initialize the distance matrix and the successor matrix.
    */
   void initRoyFloydWarshallMatrices();

   /**
    * Clean up the distance and successor matrices.
    */
   void deleteRoyFloydWarshallMatrices();
   
   /**
    * Clean up the map of tile states.
    */
   void deleteCollisionMap();

   /**
    * If an area is available, occupy it and set the tiles within it to the new state. 
    * NOTE: This is an all-or-nothing operation, which means that if there is anything blocking the area from being occupied, the entire area will be unmodified. If the area can be occupied, it will be occupied completely.
    *
    * @param area The coordinates of the top-left corner of the area to occupy (in pixels)
    * @param width The width of the area to occupy (in pixels)
    * @param height The height of the area to occupy (in pixels)
    * @param state The new state of the area (occupant and type)
    *
    * @return true if the area has been successfully occupied, false if there was something else in the area.
    */
   bool occupyArea(Point2D area, int width, int height, TileState state);

   /**
    * Free the tiles belonging to a given entity within a specified area. 
    *
    * @param area The coordinates of the top-left corner of the area to free (in pixels)
    * @param width The width of the area to free (in pixels)
    * @param height The height of the area to free (in pixels)
    */
   void freeArea(Point2D area, int width, int height);

   /**
    * Helper function to unconditionally set the tiles in an area to a given state. 
    *
    * @param left The x-coordinate of the left edge of the area to set (in tiles)
    * @param top The y-coordinate of the top edge of the area to set (in tiles)
    * @param right The x-coordinate of the right edge of the area to set (in tiles)
    * @param bottom The y-coordinate of the bottom edge of the area to set (in tiles)
    * @param state The new state of the area (occupant and type)
    */
   void setArea(int left, int top, int right, int bottom, TileState state);

   public:
      /** A set of waypoints to move through in order to go from one point to another. */
      typedef std::list<Point2D> Path;

      /**
       * Constructor.
       */
      Pathfinder();
      
      /**
       * @return The map data that the Pathfinder is operating on.
       */
      const Map* getMapData() const;
      
      /**
       * Sets a new map to operate on. Initializes the collision map and
       * runs computations on it to inform heuristics for best path calculations.
       *
       * @param newMapData The new map to operate on.
       */
      void setMapData(const Map* newMapData);
   
      /**
       * @return The name of the map.
       */
      std::string getName() const;
      
      /**
       * @return The width of the map.
       */
      int getWidth() const;
      
      /**
       * @return The height of the map.
       */
      int getHeight() const;

      /**
       * Process logic for the map and its obstacles.
       */
      void step(long timePassed);
   
      /**
       * Finds an ideal path from the source coordinates to the destination.
       *
       * @param src The coordinates of the source (in pixels).
       * @param dst The coordinates of the destination (in pixels).
       *
       * @return The ideal best path from the source point to the destination point.
       */
      Path findBestPath(Point2D src, Point2D dst);
      
      /**
       * Finds the shortest path from the source coordinates to the destination
       * around all obstacles and entities.
       *
       * @param src The coordinates of the source (in pixels).
       * @param dst The coordinates of the destination (in pixels).
       *
       * @return The shortest unobstructed path from the source point to the destination point.
       */
      Path findReroutedPath(Point2D src, Point2D dst);
      
      /**
       * Checks an area for obstacles or entities.
       *
       * @param area The coordinates of the top-left corner of the area (in pixels)
       * @param width The width of the area (in pixels)
       * @param height The height of the area (in pixels)
       *
       * @return true iff a given area is entirely free of obstacles and entities.
       */
      bool isAreaFree(Point2D area, int width, int height) const;
   
      /**
       * Add an obstacle and occupy the tiles under it.
       * NOTE: This is an all-or-nothing operation, which means that if there is anything blocking the area from being occupied, the entire area will be unmodified. If the area can be occupied, it will be occupied completely.
       *
       * @param area The coordinates of the top-left corner of the obstacle (in pixels)
       * @param width The width of the obstacle (in pixels)
       * @param height The height of the obstacle (in pixels)
       *
       * @return true if the obstacle has been successfully placed in the area, false if there was something else in the area.
       */
      bool addObstacle(Point2D area, int width, int height);

      /**
       * Add an NPC and occupy the tiles under it.
       * NOTE: This is an all-or-nothing operation, which means that if there is anything blocking the area from being occupied, the entire area will be unmodified. If the area can be occupied, it will be occupied completely.
       *
       * @param npc The NPC to add.
       * @param area The coordinates of the top-left corner of the obstacle (in pixels)
       * @param width The width of the obstacle (in pixels)
       * @param height The height of the obstacle (in pixels)
       *
       * @return true if the NPC has been successfully placed in the area, false if there was something else in the area.
       */
      bool addNPC(NPC* npc, Point2D area, int width, int height);
   
      /**
       * Request permission from the Pathfinder to move an NPC from the source to the given destination.
       * NOTE: After the NPC has completed this movement, endMovement MUST be called in order to notify the Pathfinder to perform the appropriate clean-up.
       *
       * @param npc The NPC that is moving.
       * @param src The coordinates of the source (in pixels).
       * @param dst The coordinates of the destination (in pixels).
       * @param width Width of the NPC.
       * @param height Height of the NPC.
       *
       * @return true iff the NPC can move from the source to the destination.
       */
      bool beginMovement(NPC* npc, Point2D src, Point2D dst, int width, int height);

      /**
       * Notifies the Pathfinder that the NPC moved successfully from the source to the given destination and no longer occupies the source coordinates.
       *
       * @param src The coordinates of the source (in pixels).
       * @param dst The coordinates of the destination (in pixels).
       * @param width Width of the entity that moved.
       * @param height Height of the entity that moved.
       */
      void endMovement(Point2D src, Point2D dst, int width, int height);

      /**
       * Draw the collision map for diagnostic purposes.
       */
      void draw();

      /**
       * Destructor.
       */
      ~Pathfinder();

   private:
      /**
       * @param src The coordinates of the source (in pixels).
       * @param dst The coordinates of the destination (in pixels).

       * @return A straight path from origin to goal, regardless of anything being in the way.
       */
      Path getStraightPath(Point2D src, Point2D dst);

      /**
       * Uses the successor matrix computed on Pathfinder construction to determine the best path.
       * This path does not take into account moving entities like NPCs or the player, and does not take dynamically added obstacles into account.
       *
       * @param src The coordinates of the source (in pixels).
       * @param dst The coordinates of the destination (in pixels).
       *
       * @return The best path computed by the Roy-Floyd-Warshall algorithm. 
       */
      Path findRFWPath(Point2D src, Point2D dst);

      /**
       * Uses the A* algorithm to dynamically find the best possible path. Uses the Roy-Floyd-Warshall distance matrix as a heuristic when determining the best path.
       * This path will route around any dynamically added obstacles or moving entities based on their locations when this function is called.
       *
       * @param src The coordinates of the source (in pixels).
       * @param dst The coordinates of the destination (in pixels).
       *
       * @return The best path computed by the A* algorithm.
       */
      Path findAStarPath(Point2D src, Point2D dst);

      /**
       * A node used in A* search.
       */
      class AStarPoint;

      /**
       * Evaluate a group of neighbour nodes in A* search.
       * Alters costs in the open set if cheaper paths are found,
       * and adds undiscovered tiles to the open set.
       *
       * @param adjacentNodes The neighbour nodes to evaluate.
       * @param evaluatedPoint The point that is currently being evaluated.
       * @param traversalCost The cost to move from evaluatedPoint to any of the adjacent nodes.
       * @param destinationTileNum The tile number of the goal point.
       * @param openSet The open set used to accumulate undiscovered points.
       * @param discovered A mapping from tile numbers to whether or not they have been discovered.
       */
      void evaluateAdjacentNodes(const std::vector<Point2D>& adjacentNodes, const AStarPoint* evaluatedPoint, float traversalCost, int destinationTileNum, std::vector<AStarPoint*>& openSet, std::vector<bool>& discovered);
};

#endif
