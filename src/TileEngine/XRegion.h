#ifndef __XREGION_H_
#define __XREGION_H_

#include "Region.h"
#include <string>
#include <map>

class Map;
class XMap;

/**
 * A Region is a large spatial area that the player can walk around within.
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
class XRegion : public Region
{
   protected:
      /**
       * Loads this region from the specified EDR file.
       *
       * @param path The path to the file containing the region and its maps.
       */
      virtual void load(const char* path);

   public:
      /**
       * Constructor.
       */
      XRegion(const ResourceKey& name);
};

#endif
