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
    { BLOCK_AIR                   , "assets/textures/sky.jpg" },
    { BLOCK_BRICKS                , "assets/textures/bricks.png" },
    { BLOCK_CHISELED_STONE_BRICKS , "assets/textures/chiseled_stone_bricks.png" },
    { BLOCK_COBBLESTONE           , "assets/textures/cobblestone.png" },
    { BLOCK_DIRT                  , "assets/textures/dirt.png" },
    { BLOCK_GRASS                 , "assets/textures/grass.png" },
    { BLOCK_MIRROR                , "assets/textures/mirror.png" },
    { BLOCK_OAK_LOG               , "assets/textures/oak_log.png" },
    { BLOCK_OAK_PLANKS            , "assets/textures/oak_planks.png" },
    { BLOCK_STONE                 , "assets/textures/stone.png" },
    { BLOCK_STONE_BRICKS          , "assets/textures/stone_bricks.png" },
};

constexpr size_t TEXTURES_NUM = sizeof(TEXTURES_LOCATION) / sizeof(Texture);

} // namespace puza

#endif // TEXTURE_H