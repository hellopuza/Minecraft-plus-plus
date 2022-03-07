#ifndef MATERIAL_H
#define MATERIAL_H

#include "Texture.h"

#include <iostream>

namespace puza {

struct Material final
{
    sf::Texture texture;
    const char* name;
    float brightness;
    float reflection;
    float flare_size;

    Material(const Texture& texture_, const char* name_, float brightness_ = 1.0f, float reflection_ = 0.0f, float flare_size_ = 25.0f) :
        name(name_),
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
    { TEXTURES_LOCATION[BLOCK_AIR]                   , "Air",                   0.00f, 0.00f, 0.00f },
    { TEXTURES_LOCATION[BLOCK_BRICKS]                , "Bricks",                0.13f, 0.01f, 0.03f },
    { TEXTURES_LOCATION[BLOCK_CHISELED_STONE_BRICKS] , "Chiseled stone bricks", 0.13f, 0.01f, 0.03f },
    { TEXTURES_LOCATION[BLOCK_COBBLESTONE]           , "Cobblestone",           0.13f, 0.01f, 0.03f },
    { TEXTURES_LOCATION[BLOCK_DIRT]                  , "Dirt",                  0.13f, 0.01f, 0.03f },
    { TEXTURES_LOCATION[BLOCK_GRASS]                 , "Grass",                 0.13f, 0.01f, 0.03f },
    { TEXTURES_LOCATION[BLOCK_MIRROR]                , "Mirror",                0.00f, 0.93f, 0.01f },
    { TEXTURES_LOCATION[BLOCK_OAK_LOG]               , "Oak log",               0.13f, 0.00f, 0.03f },
    { TEXTURES_LOCATION[BLOCK_OAK_PLANKS]            , "Oak planks",            0.13f, 0.00f, 0.03f },
    { TEXTURES_LOCATION[BLOCK_STONE]                 , "Stone",                 0.13f, 0.00f, 0.03f },
    { TEXTURES_LOCATION[BLOCK_STONE_BRICKS]          , "Stone bricks",          0.13f, 0.00f, 0.03f },
};

constexpr size_t MATERIALS_NUM = sizeof(MATERIALS) / sizeof(Material);

} // namespace puza

#endif // MATERIAL_H