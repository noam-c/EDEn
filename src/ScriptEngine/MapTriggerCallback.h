/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef MAP_TRIGGER_CALLBACK_H
#define MAP_TRIGGER_CALLBACK_H

class GridActor;
struct lua_State;

/**
 * Encapsulates a script function callback that is
 * executed when an Actor steps into a trigger zone on
 * the map.
 *
 * @author Noam Chitayat
 */
struct MapTriggerCallback final
{
   /** The Lua VM on which to run the callback function. */
   lua_State* m_luaVM;

   /** The registry index where the callback function is stored. */
   int m_registryIndex;

   /**
    * Constructor.
    *
    * @param luaVM A Lua VM with the callback function on top of its stack.
    */
   MapTriggerCallback(lua_State* luaVM);

   /**
    * Cleans up the script callback function.
    */
   ~MapTriggerCallback();

   /**
    * Executes the callback, supplying the Actor that triggered it.
    *
    * @param actor The Actor that stepped into the map trigger zone.
    */
   int operator()(GridActor* actor);
};

#endif
