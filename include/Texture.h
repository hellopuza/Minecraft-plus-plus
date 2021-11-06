#ifndef TEXTURE_H
#define TEXTURE_H

#include "BlockEnum.h"

namespace puza {

struct Texture final
{
    block_t     block;
    const char* location;
};

const Texture TEXTURES_LOCATION[] =
{
    { BLOCK_AIR                   , "textures/sky.jpg" },
    { BLOCK_BRICKS                , "textures/bricks.png" },
    { BLOCK_CHISELED_STONE_BRICKS , "textures/chiseled_stone_bricks.png" },
    { BLOCK_COBBLESTONE           , "textures/cobblestone.png" },
    { BLOCK_DIRT                  , "textures/dirt.png" },
    { BLOCK_GRASS                 , "textures/grass.png" },
    { BLOCK_MIRROR                , "textures/mirror.png" },
    { BLOCK_OAK_LOG               , "textures/oak_log.png" },
    { BLOCK_OAK_PLANKS            , "textures/oak_planks.png" },
    { BLOCK_STONE                 , "textures/stone.png" },
    { BLOCK_STONE_BRICKS          , "textures/stone_bricks.png" },
};

constexpr size_t TEXTURES_NUM = sizeof(TEXTURES_LOCATION) / sizeof(Texture);

} // namespace puza

#endif // TEXTURE_H