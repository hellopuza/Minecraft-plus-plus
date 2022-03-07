#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "Intersection.h"
#include "Material.h"
#include "Camera.h"
#include "Light.h"
#include "Utils.h"

namespace puza {

class RayTracer final
{
public:

    RayTracer(const vec2u& winsizes, World* world, Camera* camera, size_t ray_depth, size_t ray_range);

    void updateWinSizes(const vec2u& winsizes);

    void draw(sf::RenderTarget& target);

    void addLight(const vec3f& position, const rgb& color, float diff_intensity, float spec_intensity);

    void setLightPosition     (const vec3f& position, unsigned int i);
    void setLightColor        (const rgb&   color,    unsigned int i);
    void setLightDiffIntensity(float diff_intensity, unsigned int i);
    void setLightSpecIntensity(float spec_intensity, unsigned int i);

    void makeShader();

private:

    char* writeShader();
    static char* writeTextureSwitch();
    char* writeLightsLoop() const;
    char* writeRenderPointLoop() const;

    void setCamera();
    void setScene();
    void setMaterials();

    vec2f winsizes_;
    size_t ray_depth_;
    size_t ray_range_;

    std::vector<Light> lights_;

    World*  world_;
    Camera* camera_;

    sf::Shader shader_;
    sf::RenderTexture render_texture_;

public:

    sf::Sprite frame_;
};

} // namespace puza

#endif // RAYTRACER_H