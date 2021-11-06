#include "World.h"

#include <fstream>

namespace puza {

World::World(const vec3u& size) :
    size_     (size),
    half_size_(size / 2)
{
    map_size_ = vec2u(size_.x * size_.y / 4, size_.z);
    map2change_.create(map_size_.x, map_size_.y, sf::Color(0, 0, 0, 0));
}

World::World(const char* location)
{
    std::fstream world_file(location, std::ios::in | std::ios::binary);
    world_file.read(reinterpret_cast<char*>(&size_), sizeof(size_));

    half_size_ = size_ / 2;
    map_size_ = vec2u(size_.x * size_.y / 4, size_.z);
    map2change_.create(map_size_.x, map_size_.y, sf::Color(0, 0, 0, 0));

    for (unsigned int i = 0; i < map_size_.x * map_size_.y; i++)
    {
        sf::Color block_quarter;
        world_file.read(reinterpret_cast<char*>(&block_quarter), sizeof(block_quarter));
        map2change_.setPixel(i % map_size_.x, i / map_size_.x, block_quarter);
    }
    world_file.close();
}

void World::save(const char* location) const
{
    std::fstream world_file(location, std::ios::out | std::ios::binary);
    world_file.write(reinterpret_cast<const char*>(&size_), sizeof(size_));

    for (unsigned int i = 0; i < map_size_.x * map_size_.y; i++)
    {
        sf::Color block_quarter = map2change_.getPixel(i % map_size_.x, i / map_size_.x);
        world_file.write(reinterpret_cast<const char*>(&block_quarter), sizeof(block_quarter));
    }
    world_file.close();
}

void World::update()
{
    map2load_.loadFromImage(map2change_);
}

block_t World::getBlock(unsigned int x, unsigned int y, unsigned int z) const
{
    unsigned int offset = (z * size_.x * size_.y + y * size_.x + x) / 4;
    vec2u block_pos(offset % map_size_.x, offset / map_size_.x);

    block_t ret = 0;
    switch (x % 4)
    {
    case 0: ret = map2change_.getPixel(block_pos.x, block_pos.y).r; break;
    case 1: ret = map2change_.getPixel(block_pos.x, block_pos.y).g; break;
    case 2: ret = map2change_.getPixel(block_pos.x, block_pos.y).b; break;
    case 3: ret = map2change_.getPixel(block_pos.x, block_pos.y).a; break;
    }
    return ret;
}

void World::setBlock(unsigned int x, unsigned int y, unsigned int z, block_t block_id)
{
    unsigned int offset = (z * size_.x * size_.y + y * size_.x + x) / 4;
    vec2u block_pos(offset % map_size_.x, offset / map_size_.x);

    sf::Color block_quarter = map2change_.getPixel(block_pos.x, block_pos.y);

    switch (x % 4)
    {
    case 0: block_quarter = sf::Color(block_id, block_quarter.g, block_quarter.b, block_quarter.a); break;
    case 1: block_quarter = sf::Color(block_quarter.r, block_id, block_quarter.b, block_quarter.a); break;
    case 2: block_quarter = sf::Color(block_quarter.r, block_quarter.g, block_id, block_quarter.a); break;
    case 3: block_quarter = sf::Color(block_quarter.r, block_quarter.g, block_quarter.b, block_id); break;
    }
    map2change_.setPixel(block_pos.x, block_pos.y, block_quarter);
}

block_t World::getBlock(const vec3u& pos) const
{
    return getBlock(pos.x, pos.y, pos.z);
}

void World::setBlock(const vec3u& pos, block_t block_id)
{
    setBlock(pos.x, pos.y, pos.z, block_id);
}

vec3u World::size() const
{
    return size_;
}

vec3u World::half_size() const
{
    return half_size_;
}

const sf::Texture& World::getMap() const
{
    return map2load_;
}

vec2u World::getMapSize() const
{
    return map_size_;
}

} // namespace puza