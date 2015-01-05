/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#ifndef MAP_EXIT_MESSAGE_H
#define MAP_EXIT_MESSAGE_H

class MapExit;

struct MapExitMessage
{
   const MapExit& mapExit;

   MapExitMessage(const MapExit& mapExit);
};

#endif
