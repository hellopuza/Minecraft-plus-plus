#ifndef WORLD_H
#define WORLD_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <limits>

#include "BlockEnum.h"
#include "vec3.h"
#include "vec2.h"

namespace puza {

const vec2u START_CHUNK_POS = vec2u(1 << 12, 1 << 12);

const vec3u WORLD_CHUNK_SIZE = vec3u(256, 256, 128);
const vec2u WORLD_MAP_SIZE   = vec2u(WORLD_CHUNK_SIZE.x * WORLD_CHUNK_SIZE.y / 4U, WORLD_CHUNK_SIZE.z);

class World final
{
public:

    World() = default;

    void create(const char* location);
    vec3f load(const char* location);
    void save() const;
    void update(vec3f person_position);

    block_t getBlock(unsigned int x, unsigned int y, unsigned int z);
    void    setBlock(unsigned int x, unsigned int y, unsigned int z, block_t block_id);

    block_t getBlock(const vec3u& pos);
    void    setBlock(const vec3u& pos, block_t block_id);

    vec2u getCenterChunkPos() const;

    const sf::Texture& getMap_0();
    const sf::Texture& getMap_1();
    const sf::Texture& getMap_2();
    const sf::Texture& getMap_3();

private:

    void addChunk(vec2u chunk_pos);

    struct Chunk
    {
        sf::Texture downloadable_map;
        sf::Image alterable_map;

        Chunk();

        void update();
        void create();
        block_t getBlock(unsigned int x, unsigned int y, unsigned int z) const;
        void    setBlock(unsigned int x, unsigned int y, unsigned int z, block_t block_id);
    };

    std::unordered_map<vec2u, Chunk> chunks_;
    std::string location_;

    vec3f person_position_;
    vec2u current_chunk_pos_;
    vec2u center_chunk_pos_;
};

} // namespace puza

#endif // WORLD_H