/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef ENTITY_GRID_H
#define ENTITY_GRID_H

#include <limits>
#include <string>
#include <list>
#include <vector>
#include "Grid.h"
#include "MovementDirection.h"
#include "Pathfinder.h"
#include "Rectangle.h"
#include "Listener.h"

class Obstacle;
class Map;
class Actor;
class TileEngine;
class TriggerZone;

struct ActorMoveMessage;

namespace messaging
{
   class MessagePipe;
};

namespace shapes
{
   struct Size;
   struct Point2D;
};

struct TileState;

/**
 * The EntityGrid class binds to a Map and stores the locations of entities on top of it.
 * EntityGrid instances also provide an interface to entities like the actor and PlayerCharacter to detect collisions
 * and route around them.
 *
 * @author Noam Chitayat
 */
class EntityGrid : messaging::Listener<ActorMoveMessage>
{
   friend class Pathfinder;
   
   /** The size of a movement tile (used to control pathfinding granularity) */
   static const int MOVEMENT_TILE_SIZE;

   /** The square root of 2. */
   static const float ROOT_2;

   /** Floating-point notation for infinity. */
   static const float INFINITY;
   
   /** The tile engine that moderates this grid. */
   const TileEngine& m_tileEngine;

   /** The message pipe used for trigger events. */
   messaging::MessagePipe& m_messagePipe;

   /** The map on which the grid is overlaid. */
   std::weak_ptr<const Map> m_map;

   /** The pathfinding component used to navigate in this map. */
   Pathfinder m_pathfinder;
   
   /** The map of entities and states for each of the tiles. */
   Grid<TileState> m_collisionMap;

   /** The bounds of the pathfinder map. */
   shapes::Rectangle m_collisionMapBounds;

   /**
    * Clean up the map of tile states.
    */
   void deleteCollisionMap();
   
   /**
    * @param area The pixel-coordinate rectangle to determine boundaries for.
    *
    * @return Get the tile boundaries of the specified rectangle.
    */
   shapes::Rectangle getCollisionMapEdges(const shapes::Rectangle& area) const;

   /**
    * Checks if an area is available.
    *
    * @param area The rectangular region to occupy (in pixels)
    * @param state The new state of the area (entity and type)
    *
    * @return true if the area can be successfully occupied, false if there was something else in the area.
    */
   bool canOccupyArea(const shapes::Rectangle& area, TileState state) const;
   
   /**
    * If an area is available, occupy it and set the tiles within it to the new state. 
    * NOTE: This is an all-or-nothing operation, which means that if there is anything blocking the area from being occupied, the entire area will be unmodified. If the area can be occupied, it will be occupied completely.
    *
    * @param area The rectangular region to occupy (in pixels)
    * @param state The new state of the area (entity and type)
    *
    * @return true if the area has been successfully occupied, false if there was something else in the area.
    */
   bool occupyArea(const shapes::Rectangle& area, TileState state);

   /**
    * Free the tiles belonging to a given entity within a specified area. 
    *
    * @param areaToFree The rectangular region to free (in pixels)
    */
   void freeArea(const shapes::Rectangle& areaToFree);
   
   /**
    * Free the tiles belonging to a given entity within a specified area. 
    *
    * @param previousLocation The coordinates of the top-left corner of the area to free (in pixels)
    * @param currentLocation The coordinates of the top-left corner of the area to keep in the current state (in pixels)
    * @param size The size of the area to free (in pixels)
    * @param state The state to remove from the tiles
    */
   void freeArea(const shapes::Point2D& previousLocation, const shapes::Point2D& currentLocation, const shapes::Size& size, TileState state);

   /**
    * Helper function to unconditionally set the tiles in an area to a given state. 
    *
    * @param area The rectangular area to set (with edge coordinates in tiles)
    * @param state The new state of the area (entity and type)
    */
   void setArea(const shapes::Rectangle& area, TileState state);

   public:
      /** A set of waypoints to move through in order to go from one point to another. */
      typedef std::list<shapes::Point2D> Path;

      /**
       * Constructor.
       *
       * @param tileEngine The tile engine that owns this entity grid.
       * @param messagePipe The message pipe to use for trigger messages.
       */
      EntityGrid(const TileEngine& tileEngine, messaging::MessagePipe& messagePipe);

      /**
       * Destructor.
       */
      ~EntityGrid();
   
      /**
       * Gets the entry point when entering this map from another specified map.
       *
       * @param exitedMapName The name of the map that the player is coming from.
       *
       * @return The entry point to the map from the given map.
       */
      const shapes::Point2D& getMapEntrance(const std::string& exitedMapName) const;
   
      /**
       * @return True iff the EntityGrid is operating on defined and valid map data.
       */
      bool hasMapData() const;
   
      /**
       * @return The name of the map.
       * @throws
       */
      const std::string& getMapName() const;

      /**
       * Sets a new map to operate on. Initializes the collision map and
       * runs computations on it to inform heuristics for best path calculations.
       *
       * @param map The new map to operate on.
       */
      void setMapData(std::weak_ptr<const Map> map);
      
      /**
       * @return The bounds of the map.
       */
      const shapes::Rectangle& getMapBounds() const;

      /**
       * @param point The coordinates to check (in pixels)
       *
       * @return true iff the point is within the map
       */
      bool withinMap(const shapes::Point2D& point) const;

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
      Path findBestPath(const shapes::Point2D& src, const shapes::Point2D& dst);
      
      /**
       * Finds the shortest path from the source coordinates to the destination
       * around all obstacles and entities.
       *
       * @param src The coordinates of the source (in pixels).
       * @param dst The coordinates of the destination (in pixels).
       * @param size The size of the moving entity.
       *
       * @return The shortest unobstructed path from the source point to the destination point.
       */
      Path findReroutedPath(const shapes::Point2D& src, const shapes::Point2D& dst, const shapes::Size& size);
      
      /**
       * Checks an area for obstacles or entities.
       *
       * @param area The area to check (in pixels)
       *
       * @return true iff a given area is entirely free of obstacles and entities.
       */
      bool isAreaFree(const shapes::Rectangle& area) const;
   
      /**
       * Add an obstacle and occupy the tiles under it.
       * NOTE: This is an all-or-nothing operation, which means that if there is anything blocking the area from being occupied, the entire area will be unmodified. If the area can be occupied, it will be occupied completely.
       *
       * @param area The coordinates of the top-left corner of the obstacle (in pixels)
       * @param size The size of the obstacle (in pixels)
       *
       * @return true if the obstacle has been successfully placed in the area, false if there was something else in the area.
       */
      bool addObstacle(const shapes::Point2D& area, const shapes::Size& size);

      /**
       * Add an Actor and occupy the tiles under it.
       * NOTE: This is an all-or-nothing operation, which means that if there is anything blocking the area from being occupied, the entire area will be unmodified. If the area can be occupied, it will be occupied completely.
       *
       * @param actor The actor to add.
       * @param area The area to add the actor to.
       *
       * @return true if the actor has been successfully placed in the area, false if there was something else in the area.
       */
      bool addActor(Actor* actor, const shapes::Point2D& area);

      /**
       * Change the actor location, if the destination tiles are available to occupy.
       * NOTE: This is an all-or-nothing operation, which means that if there is anything blocking the destination from being occupied, the entire area will be unmodified. If the area can be occupied, it will be occupied completely.
       *
       * @param actor The actor to relocate.
       * @param dst The coordinates to relocate the actor to (in pixels)
       *
       * @return true if the player character has been successfully moved to the destination, false if there was something else in the destination.
       */
      bool changeActorLocation(Actor* actor, const shapes::Point2D& dst);

      /**
       * Remove the player and free the tiles under it.
       *
       * @param actor The actor that is being removed.
       */
      void removeActor(Actor* actor);

      /**
       * Gets the actor occupying the area in front of the specified actor, if one exists.
       *
       * @param actor The actor to search in front of.
       */
      Actor* getAdjacentActor(Actor* actor) const;

      /**
       * Given the distance the entity can move and the direction, moves as far as possible until an obstacle is encountered.
       *
       * @param actor The actor to move.
       * @param xDirection The direction moved on the x-axis.
       * @param yDirection The direction moved on the y-axis.
       * @param distance The total distance that the player character can be moved in this call.
       *
       * @return true iff the actor moved to a new location
       */
      bool moveToClosestPoint(Actor* actor, int xDirection, int yDirection, int distance);
   
      /**
       * Request permission from the EntityGrid to move an Actor from the source to the given destination.
       * NOTE: After the actor has completed this movement, endMovement MUST be called in order to notify the EntityGrid to perform the appropriate clean-up.
       *
       * @param actor The actor that is moving.
       * @param dst The coordinates of the destination (in pixels).
       *
       * @return true iff the actor can move from the source to the destination.
       */
      bool beginMovement(Actor* actor, const shapes::Point2D& dst);
      
      /**
       * Notifies the EntityGrid that the actor failed to complete movement from the source to the given destination and occupies some area between the source and destination.
       *
       * @param actor The actor that was moving.
       * @param src The coordinates of the source (in pixels).
       * @param dst The coordinates of the original destination (in pixels).
       */
      void abortMovement(Actor* actor, const shapes::Point2D& src, const shapes::Point2D& dst);
      
      /**
       * Notifies the EntityGrid that the actor moved successfully from the source to the given destination and no longer occupies the source coordinates.
       *
       * @param actor The actor that was moving.
       * @param src The coordinates of the source (in pixels).
       * @param dst The coordinates of the destination (in pixels).
       */
      void endMovement(Actor* actor, const shapes::Point2D& src, const shapes::Point2D& dst);
   
      /**
       * Draw a row of the background layers of the map.
       *
       * @param y The row to draw.
       */
      void drawBackground(int y) const;

      /**
       * Draw a row of the foreground layers of the map.
       *
       * @param y The row to draw.
       */
      void drawForeground(int y) const;

      /**
       * Receive location change messages.
       *
       * @param message The location change message that was fired.
       */
      void receive(const ActorMoveMessage& message);
};

#endif
