#include "World.h"

#include <fstream>
#include <cstdint>

namespace puza {

void World::create(const char* location)
{
    location_ = location;
    
    Chunk chunk;
    chunk.create();
    chunk.update();
    chunks_[START_CHUNK_POS] = chunk;
}

vec3f World::load(const char* location)
{
    location_ = location;
    std::fstream world_file(location_, std::ios::in | std::ios::binary);

    world_file.read(reinterpret_cast<char*>(&person_position_), sizeof(person_position_));

    uint32_t chunks_num;
    world_file.read(reinterpret_cast<char*>(&chunks_num), sizeof(chunks_num));

    for (uint32_t i = 0; i < chunks_num; i++)
    {
        Chunk chunk;

        vec2<uint32_t> chunk_pos;
        world_file.read(reinterpret_cast<char*>(&chunk_pos), sizeof(chunk_pos));

        for (unsigned int i = 0; i < WORLD_MAP_SIZE.x * WORLD_MAP_SIZE.y; i++)
        {
            sf::Color block_quarter;
            world_file.read(reinterpret_cast<char*>(&block_quarter), sizeof(block_quarter));
            chunk.alterable_map.setPixel(i % WORLD_MAP_SIZE.x, i / WORLD_MAP_SIZE.x, block_quarter);
        }
        chunk.update();

        chunks_[chunk_pos] = chunk;
    }

    world_file.close();
    return person_position_;
}

void World::save() const
{
    std::fstream world_file(location_, std::ios::out | std::ios::binary);

    world_file.write(reinterpret_cast<const char*>(&person_position_), sizeof(person_position_));

    auto chunks_num = static_cast<uint32_t>(chunks_.size());
    world_file.write(reinterpret_cast<char*>(&chunks_num), sizeof(chunks_num));

    for (const auto& [key, chunk] : chunks_)
    {
        vec2<uint32_t> chunk_pos(key);
        world_file.write(reinterpret_cast<char*>(&chunk_pos), sizeof(chunk_pos));

        for (unsigned int i = 0; i < WORLD_MAP_SIZE.x * WORLD_MAP_SIZE.y; i++)
        {
            sf::Color block_quarter = chunk.alterable_map.getPixel(i % WORLD_MAP_SIZE.x, i / WORLD_MAP_SIZE.x);
            world_file.write(reinterpret_cast<char*>(&block_quarter), sizeof(block_quarter));
        }
    }

    world_file.close();
}

void World::update(vec3f person_position)
{
    person_position_ = person_position;

    vec2f chunks_offset(person_position_.x / static_cast<float>(WORLD_CHUNK_SIZE.x),
                        person_position_.y / static_cast<float>(WORLD_CHUNK_SIZE.y));

    current_chunk_pos_ = vec2u(static_cast<unsigned int>(chunks_offset.x) * WORLD_CHUNK_SIZE.x,
                               static_cast<unsigned int>(chunks_offset.y) * WORLD_CHUNK_SIZE.y);

    vec2u chunks_quarter_offset(chunks_offset * 2.0F);
    center_chunk_pos_ = vec2u(current_chunk_pos_.x + WORLD_CHUNK_SIZE.x * (chunks_quarter_offset.x % 2U),
                              current_chunk_pos_.y + WORLD_CHUNK_SIZE.y * (chunks_quarter_offset.y % 2U));
}

block_t World::getBlock(unsigned int x, unsigned int y, unsigned int z)
{
    vec2u chunk_pos((x / WORLD_CHUNK_SIZE.x) * WORLD_CHUNK_SIZE.x, (y / WORLD_CHUNK_SIZE.y) * WORLD_CHUNK_SIZE.y);
    return chunks_[chunk_pos].getBlock(x - chunk_pos.x, y - chunk_pos.y, z);
}

void World::setBlock(unsigned int x, unsigned int y, unsigned int z, block_t block_id)
{
    vec2u chunk_pos((x / WORLD_CHUNK_SIZE.x) * WORLD_CHUNK_SIZE.x, (y / WORLD_CHUNK_SIZE.y) * WORLD_CHUNK_SIZE.y);
    Chunk* chunk = &chunks_[chunk_pos];
    chunk->setBlock(x - chunk_pos.x, y - chunk_pos.y, z, block_id);
    chunk->update();
}

block_t World::getBlock(const vec3u& pos)
{
    return getBlock(pos.x, pos.y, pos.z);
}

void World::setBlock(const vec3u& pos, block_t block_id)
{
    setBlock(pos.x, pos.y, pos.z, block_id);
}

block_t World::Chunk::getBlock(unsigned int x, unsigned int y, unsigned int z) const
{
    unsigned int offset = (z * WORLD_CHUNK_SIZE.x * WORLD_CHUNK_SIZE.y + y * WORLD_CHUNK_SIZE.x + x) / 4U;
    vec2u block_pos(offset % WORLD_MAP_SIZE.x, offset / WORLD_MAP_SIZE.x);

    block_t ret = 0;
    switch (x % 4)
    {
    case 0: ret = alterable_map.getPixel(block_pos.x, block_pos.y).r; break;
    case 1: ret = alterable_map.getPixel(block_pos.x, block_pos.y).g; break;
    case 2: ret = alterable_map.getPixel(block_pos.x, block_pos.y).b; break;
    case 3: ret = alterable_map.getPixel(block_pos.x, block_pos.y).a; break;
    }
    return ret;
}

void World::Chunk::setBlock(unsigned int x, unsigned int y, unsigned int z, block_t block_id)
{
    unsigned int offset = (z * WORLD_CHUNK_SIZE.x * WORLD_CHUNK_SIZE.y + y * WORLD_CHUNK_SIZE.x + x) / 4U;
    vec2u block_pos(offset % WORLD_MAP_SIZE.x, offset / WORLD_MAP_SIZE.x);

    sf::Color block_quarter = alterable_map.getPixel(block_pos.x, block_pos.y);

    switch (x % 4)
    {
    case 0: block_quarter = sf::Color(block_id, block_quarter.g, block_quarter.b, block_quarter.a); break;
    case 1: block_quarter = sf::Color(block_quarter.r, block_id, block_quarter.b, block_quarter.a); break;
    case 2: block_quarter = sf::Color(block_quarter.r, block_quarter.g, block_id, block_quarter.a); break;
    case 3: block_quarter = sf::Color(block_quarter.r, block_quarter.g, block_quarter.b, block_id); break;
    }

    alterable_map.setPixel(block_pos.x, block_pos.y, block_quarter);
}

void World::addChunk(vec2u chunk_pos)
{
    auto search = chunks_.find(chunk_pos);
    if (search == chunks_.end())
    {
        Chunk chunk;
        chunk.create();
        chunk.update();
        chunks_[chunk_pos] = chunk;
    }
}

vec2u World::getCenterChunkPos() const
{
    return center_chunk_pos_;
}

const sf::Texture& World::getMap_0()
{
    vec2u chunk_pos(center_chunk_pos_.x - WORLD_CHUNK_SIZE.x, center_chunk_pos_.y);
    addChunk(chunk_pos);
    return chunks_[chunk_pos].downloadable_map;
}

const sf::Texture& World::getMap_1()
{
    addChunk(center_chunk_pos_);
    return chunks_[center_chunk_pos_].downloadable_map;
}

const sf::Texture& World::getMap_2()
{
    vec2u chunk_pos(center_chunk_pos_.x - WORLD_CHUNK_SIZE.x, center_chunk_pos_.y - WORLD_CHUNK_SIZE.y);
    addChunk(chunk_pos);
    return chunks_[chunk_pos].downloadable_map;
}

const sf::Texture& World::getMap_3()
{
    vec2u chunk_pos(center_chunk_pos_.x, center_chunk_pos_.y - WORLD_CHUNK_SIZE.y);
    addChunk(chunk_pos);
    return chunks_[chunk_pos].downloadable_map;
}

World::Chunk::Chunk()
{
    alterable_map.create(WORLD_MAP_SIZE.x, WORLD_MAP_SIZE.y, sf::Color(0, 0, 0, 0));
}

void World::Chunk::update()
{
    downloadable_map.loadFromImage(alterable_map);
}

void World::Chunk::create()
{
    for (unsigned int x = 0; x < WORLD_CHUNK_SIZE.x; x++)
    {
        for (unsigned int y = 0; y < WORLD_CHUNK_SIZE.y; y++)
        {
            setBlock(x, y, 0U, BLOCK_GRASS);
        }
    }
}

} // namespace puza