/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef REGION_H
#define REGION_H

#include "Resource.h"
#include <map>
#include <memory>
#include <string>

class Map;

/**
 * A Region is a large spatial area within which the player can freely move.
 * Technically speaking, it's a set of Map instances that are tied together
 * as a cohesive unit that is loaded all at once to allow seamless transitioning
 * between locations in the same area, such as different houses in a town,
 * or different levels of a single dungeon.
 * A Region contains a series of Maps keyed by their names.
 * The first map loaded from file becomes the starting map, and the player
 * character begins there when entering a region unless otherwise specified.
 *
 * @author Noam Chitayat
 */
class Region : public Resource
{
   /** The name of the region. */
   std::string m_regionName;

   /** The list of maps in this region, keyed by map names. */
   std::map<std::string, std::shared_ptr<Map>> m_areas;

   /**
    * Loads this region from the specified directory.
    *
    * @param path The path to the directory containing the region's maps.
    */
   void load(const std::string& path) override;

   public:
      /**
       * Constructor.
       *
       * @param name The name of the region resource.
       */
      Region(const ResourceKey& name);

      ~Region() override;

      /** @return the name of the region. */
      std::string getName() const;

      /** @return the starting map (first map loaded) for this Region. */
      std::weak_ptr<Map> getStartingMap();

      /**
       * @param name The name of the Map to retrieve.
       *
       * @return the Map with the specified name.
       */
      std::weak_ptr<Map> getMap(const std::string& name);
};

#endif
