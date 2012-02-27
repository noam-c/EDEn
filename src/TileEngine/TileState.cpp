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
