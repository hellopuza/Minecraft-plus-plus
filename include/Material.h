#ifndef MATERIAL_H
#define MATERIAL_H

#include "Texture.h"

#include <iostream>

namespace puza {

struct Material final
{
    float brightness;
    float reflection;
    float flare_size;
    sf::Texture texture;

    Material (const Texture& texture_, float brightness_ = 1.0f, float reflection_ = 0.0f, float flare_size_ = 25.0f) :
        brightness(brightness_),
        reflection(reflection_),
        flare_size(flare_size_)
    {
        if (not texture.loadFromFile(texture_.location))
            std::cerr << "Failed to load texture " << texture_.location << "\n";
    }
};

const Material MATERIALS[] =
{
    { TEXTURES_LOCATION[BLOCK_AIR]                   , 0.0f, 0.00f, 0.00f },
    { TEXTURES_LOCATION[BLOCK_BRICKS]                , 0.1f, 0.04f, 0.03f },
    { TEXTURES_LOCATION[BLOCK_CHISELED_STONE_BRICKS] , 0.1f, 0.04f, 0.03f },
    { TEXTURES_LOCATION[BLOCK_COBBLESTONE]           , 0.1f, 0.04f, 0.03f },
    { TEXTURES_LOCATION[BLOCK_DIRT]                  , 0.1f, 0.08f, 0.03f },
    { TEXTURES_LOCATION[BLOCK_GRASS]                 , 0.1f, 0.15f, 0.03f },
    { TEXTURES_LOCATION[BLOCK_MIRROR]                , 0.0f, 0.90f, 0.01f },
    { TEXTURES_LOCATION[BLOCK_OAK_LOG]               , 0.1f, 0.08f, 0.03f },
    { TEXTURES_LOCATION[BLOCK_OAK_PLANKS]            , 0.1f, 0.08f, 0.03f },
    { TEXTURES_LOCATION[BLOCK_STONE]                 , 0.1f, 0.04f, 0.03f },
    { TEXTURES_LOCATION[BLOCK_STONE_BRICKS]          , 0.1f, 0.04f, 0.03f },
};

constexpr size_t MATERIALS_NUM = sizeof(MATERIALS) / sizeof(Material);

} // namespace puza

#endif // MATERIAL_H