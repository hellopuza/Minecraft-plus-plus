#ifndef WORLD_H
#define WORLD_H

#include <SFML/Graphics.hpp>
#include "BlockEnum.h"
#include "vec3.h"
#include "vec2.h"

namespace puza {

class World final
{
public:

    World(const vec3u& size);
    World(const char* location);

    void save(const char* location) const;
    void update();

    block_t getBlock(unsigned int x, unsigned int y, unsigned int z) const;
    void    setBlock(unsigned int x, unsigned int y, unsigned int z, block_t block_id);

    block_t getBlock(const vec3u& pos) const;
    void    setBlock(const vec3u& pos, block_t block_id);

    vec3u size() const;
    vec3u half_size() const;

    const sf::Texture& getMap() const;
    vec2u getMapSize() const;

private:

    vec3u size_;
    vec3u half_size_;
    vec2u map_size_;

    sf::Image map2change_;
    sf::Texture map2load_;
};

} // namespace puza

#endif // WORLD_H