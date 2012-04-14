/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "TileState.h"
#define NULL 0

TileState::TileState() : entityType(FREE), entity(NULL)
{
}

TileState::TileState(EntityType type) : entityType(type), entity(NULL)
{
}

TileState::TileState(EntityType type, void* entity) : entityType(type), entity(entity)
{
}
