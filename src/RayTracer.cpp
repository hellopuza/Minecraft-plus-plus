#include "RayTracer.h"
#include <cstring>

namespace puza {

RayTracer::RayTracer(const vec2u& winsizes, World* world, Camera* camera, size_t ray_depth, size_t ray_range) :
    winsizes_ (winsizes),
    ray_depth_(ray_depth),
    ray_range_(ray_range),
    world_    (world),
    camera_   (camera)
{
    render_texture_.create(winsizes.x, winsizes.y);
    sprite_ = sf::Sprite(render_texture_.getTexture());
}

void RayTracer::updateWinSizes(const vec2u& winsizes)
{
    winsizes_ = winsizes;

    render_texture_.create(winsizes.x, winsizes.y);
    sprite_ = sf::Sprite(render_texture_.getTexture());
}

void RayTracer::draw(sf::RenderWindow& window)
{
    shader_.setUniform("winsizes", sf::Glsl::Vec2(winsizes_.x, winsizes_.y));

    setMaterials();
    setCamera();
    setScene();

    render_texture_.draw(sprite_, &shader_);
    window.draw(sprite_);
}

void RayTracer::addLight(const vec3f& position, const rgb& color, float diff_intensity, float spec_intensity)
{
    lights_.emplace_back(Light(position, color, diff_intensity, spec_intensity));
}

void RayTracer::setLightPosition(const vec3f& position, unsigned int i)
{
    lights_[i].position = position;
}

void RayTracer::setLightColor(const rgb& color, unsigned int i)
{
    lights_[i].color = color;
}

void RayTracer::setLightDiffIntensity(float diff_intensity, unsigned int i)
{
    lights_[i].diffused_intensity = diff_intensity;
}

void RayTracer::setLightSpecIntensity(float spec_intensity, unsigned int i)
{
    lights_[i].specular_intensity = spec_intensity;
}

void RayTracer::makeShader()
{
    char* str_shader = writeShader();

    shader_.loadFromMemory(str_shader, sf::Shader::Fragment);

    delete [] str_shader;
}

char* RayTracer::writeShader()
{
    char* str_shader = new char[20000] {};

    char* texture_switch = writeTextureSwitch();
    char* lights_loop = writeLightsLoop();
    char* render_point_loops = writeRenderPointLoop();

    sprintf(str_shader,
        "#version 130\n"
        "\n"
        "#define rgb vec3\n"
        "const float PI = atan(1.0) * 4.0;\n"
        "\n"
        "const uint  RAY_DEPTH     = %luu;\n"                  /* 1 */
        "const uint  RAY_RANGE     = %luu;\n"                  /* 2 */
        "const uint  LIGHTS_NUM    = %luu;\n"                  /* 3 */
        "const uint  MATERIALS_NUM = %luu;\n"                  /* 4 */
        "const uvec3 WORLD_SIZE    = uvec3(%uu, %uu, %uu);\n"  /* 5, 6, 7 */
        "\n"
        "uniform vec2  winsizes;\n"
        "uniform ivec2 mapsizes;\n"
        "uniform sampler2D world;\n"
        "\n"
        "uint getBlock(uvec3 point)\n"
        "{\n"
        "    uint offset = (point.z * WORLD_SIZE.x * WORLD_SIZE.y + point.y * WORLD_SIZE.x + point.x) / 4u;\n"
        "    uvec2 block_pos = uvec2(offset %% uint(mapsizes.x), offset / uint(mapsizes.x));\n"
        "    vec4 block_quarter = texture2D(world, vec2(float(block_pos.x) / mapsizes.x, float(block_pos.y) / mapsizes.y));\n"
        "    uint block = point.x %% 4u;\n"
        "    if (block == 0u) return uint(block_quarter.x * 255.0); else\n"
        "    if (block == 1u) return uint(block_quarter.y * 255.0); else\n"
        "    if (block == 2u) return uint(block_quarter.z * 255.0); else\n"
        "    return uint(block_quarter.w * 255.0);\n"
        "}\n"
        "\n"
        "struct Material\n"
        "{\n"
        "    float brightness;\n"
        "    float reflection;\n"
        "    float flare_size;\n"
        "};\n"
        "\n"
        "uniform Material  materials[MATERIALS_NUM + 2u];\n"
        "uniform sampler2D textures [MATERIALS_NUM + 2u];\n"
        "\n"
        "rgb getBlockPixel(uint id, vec2 point)\n"
        "{\n"
        "    vec4 col;\n"
        "    switch (id)\n"
        "    {\n"
        "    %s"  // 8 texture_switch
        "    default: col = texture2D(textures[0u], point);\n"
        "    }\n"
        "    return rgb(col.x, col.y, col.z);\n"
        "}\n"
        "\n"
        "struct Ray\n"
        "{\n"
        "    vec3 origin;\n"
        "    vec3 direction;\n"
        "};\n"
        "\n"
        "vec3 RayPoint(Ray ray, float t) { return ray.origin + t * ray.direction; }\n"
        "\n"
        "struct Camera\n"
        "{\n"
        "    vec2 sizes;\n"
        "    vec3 position;\n"
        "    vec3 forward;\n"
        "    vec3 right;\n"
        "    vec3 up;\n"
        "};\n"
        "uniform Camera camera;\n"
        "\n"
        "Ray CameraMakeRay(vec2 point)\n"
        "{\n"
        "    return Ray(camera.position, normalize(camera.forward - point.x * camera.sizes.x * camera.right + point.y * camera.sizes.y * camera.up));\n"
        "}\n"
        "\n"
        "struct Intersection\n"
        "{\n"
        "    Ray   ray;\n"
        "    vec3  normal;\n"
        "    vec3  point;\n"
        "    vec3  side_centre;\n"
        "    uint  block_id;\n"
        "    float max_dist;\n"
        "} intersection;\n"
        "\n"
        "bool intersect(inout Intersection crossing)\n"
        "{\n"
        "    vec3 current_pos = crossing.ray.origin;\n"
        "\n"
        "    vec3 dir = abs(crossing.ray.direction);\n"
        "    vec3 pos_dir = vec3(float(crossing.ray.direction.x > 0.0),\n"
        "                        float(crossing.ray.direction.y > 0.0),\n"
        "                        float(crossing.ray.direction.z > 0.0));\n"
        "\n"
        "    vec3 step = vec3(2.0, 2.0, 2.0) * pos_dir - vec3(1.0, 1.0, 1.0);\n"
        "    vec3 pos_step = vec3(float(step.x > 0.0),\n"
        "                         float(step.y > 0.0),\n"
        "                         float(step.z > 0.0));\n"
        "\n"
        "    vec3 step_less_one = vec3(float(step.x < 1.0),\n"
        "                              float(step.y < 1.0),\n"
        "                              float(step.z < 1.0));\n"
        "\n"
        "    vec3 t_max = ((vec3(2.0, 2.0, 2.0) * pos_step - vec3(1.0, 1.0, 1.0)) * (vec3(ivec3(current_pos)) - current_pos) + pos_step) / dir;\n"
        "\n"
        "    for (uint i = 0u; i < RAY_RANGE; i++)\n"
        "    {\n"
        "        ivec3 min_t_max = ivec3(int((t_max.x < t_max.y) && (t_max.x < t_max.z)),\n"
        "                                int((t_max.y < t_max.x) && (t_max.y < t_max.z)),\n"
        "                                int((t_max.z < t_max.x) && (t_max.z < t_max.y)));\n"
        "\n"
        "        current_pos += step * min_t_max;\n"
        "        t_max += min_t_max / dir;\n"
        "\n"
        "        if ((current_pos.x > float(WORLD_SIZE.x)) || (current_pos.x < 0.0) ||\n"
        "            (current_pos.y > float(WORLD_SIZE.y)) || (current_pos.y < 0.0) ||\n"
        "            (current_pos.z > float(WORLD_SIZE.z)) || (current_pos.z < 0.0))\n"
        "            return false;\n"
        "        uvec3 block_pos = uvec3(current_pos);\n"
        "        uint block_id = getBlock(block_pos);\n"
        "\n"
        "        if (block_id > 0u)\n"
        "        {\n"
        "            crossing.normal = vec3(min_t_max);\n"
        "            crossing.side_centre = vec3(block_pos) + min_t_max * step_less_one + vec3(0.5, 0.5, 0.5) * (vec3(1.0, 1.0, 1.0) - min_t_max);\n"
        "\n"
        "            float dot_product = dot(crossing.ray.direction, crossing.normal);\n"
        "            crossing.point = RayPoint(crossing.ray, dot((crossing.side_centre - crossing.ray.origin), crossing.normal) / dot_product) - crossing.ray.direction * 5e-6;\n"
        "            crossing.normal = crossing.normal * (1.0 - 2.0 * float(dot_product > 0.0));\n"
        "            crossing.block_id = block_id;\n"
        "            return (length(crossing.ray.origin - crossing.point) < crossing.max_dist);\n"
        "        }\n"
        "    }\n"
        "    return false;\n"
        "}\n"
        "\n"
        "rgb getPointColor()\n"
        "{\n"
        "    vec3 point = intersection.point - intersection.side_centre;\n"
        "    vec2 texture_point;\n"
        "    if (intersection.normal.y == -1)\n"
        "        texture_point = vec2((point.x + 0.5) * 0.25 + 0.0,  (0.5 - point.z) / 3.0 + 1.0 / 3.0);\n"
        "    else\n"
        "    if (intersection.normal.x == +1)\n"
        "        texture_point = vec2((point.y + 0.5) * 0.25 + 0.25, (0.5 - point.z) / 3.0 + 1.0 / 3.0);\n"
        "    else\n"
        "    if (intersection.normal.y == +1)\n"
        "        texture_point = vec2((0.5 - point.x) * 0.25 + 0.5,  (0.5 - point.z) / 3.0 + 1.0 / 3.0);\n"
        "    else\n"
        "    if (intersection.normal.x == -1)\n"
        "        texture_point = vec2((0.5 - point.y) * 0.25 + 0.75, (0.5 - point.z) / 3.0 + 1.0 / 3.0);\n"
        "    else\n"
        "    if (intersection.normal.z == +1)\n"
        "        texture_point = vec2((point.y + 0.5) * 0.25 + 0.25, (0.5 + point.x) / 3.0);\n"
        "    else\n"
        "    if (intersection.normal.z == -1)\n"
        "        texture_point = vec2((point.y + 0.5) * 0.25 + 0.25, (0.5 - point.x) / 3.0 + 2.0 / 3.0);\n"
        "\n"
        "    return getBlockPixel(intersection.block_id, texture_point);\n"
        "}\n"
        "\n"
        "bool lightIntersect(Ray ray, float max_dist)\n"
        "{\n"
        "    Intersection to_light;\n"
        "    to_light.ray = ray;\n"
        "    to_light.max_dist = max_dist;\n"
        "    return intersect(to_light);\n"
        "}\n"
        "\n"
        "struct Light\n"
        "{\n"
        "    vec3  position;\n"
        "    rgb   color;\n"
        "    float diffused_intensity;\n"
        "    float specular_intensity;\n"
        "};\n"
        "\n"
        "uniform Light lights[LIGHTS_NUM + 2u];\n"
        "\n"
        "void enlighten(uint light_num, inout rgb colored_specular, inout rgb colored_diffused)\n"
        "{\n"
        "    Ray to_light = Ray(intersection.point, normalize(lights[light_num].position - intersection.point));\n"
        "    float specular = 0.0;\n"
        "    float diffused = 0.0;\n"
        "    if (!lightIntersect(to_light, length(intersection.point - lights[light_num].position)))\n"
        "    {\n"
        "        vec3 to_light_p = intersection.normal * dot(to_light.direction, intersection.normal) * 2.0 - to_light.direction;\n"
        "        diffused = lights[light_num].diffused_intensity * dot(to_light.direction, intersection.normal);\n"
        "        specular = -dot(to_light_p, intersection.ray.direction);\n"
        "        specular = specular * float(specular > 0.0);\n"
        "        specular = lights[light_num].specular_intensity * pow(specular, 1.0 / materials[intersection.block_id].flare_size);\n"
        "    }\n"
        "    colored_diffused += lights[light_num].color * diffused * float(diffused > 0.0);\n"
        "    colored_specular += lights[light_num].color * specular;\n"
        "}\n"
        "\n"
        "rgb RayTrace(out rgb point_color, out float k)\n"
        "{\n"
        "    if (intersect(intersection))\n"
        "    {\n"
        "        rgb diffused = rgb(0.0, 0.0, 0.0);\n"
        "        rgb specular = rgb(0.0, 0.0, 0.0);\n"
        "        %s"                         /* 9 lights_loop */
        "        k = materials[intersection.block_id].reflection;\n"
        "        float ambient = materials[intersection.block_id].brightness;\n"
        "        point_color = getPointColor();\n"
        "        rgb result = point_color * ambient + point_color * diffused * (1.0 - k) * (1.0 - ambient) + specular * k;\n"
        "        clamp(result, vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0));\n"
        "        return result;\n"
        "    }\n"
        "    else\n"
        "    {\n"
        "        vec3  point = intersection.ray.direction;\n"
        "        float rad = length(vec2(point.x, point.y));\n"
        "        float less_zero = float( !(point.y > 0));\n"
        "        float alpha = acos(point.x / rad) * (1.0 - less_zero * 2.0) + PI * 2.0 * less_zero;\n"
        "        float beta = atan(point.z / rad) + PI / 2.0;\n"
        "        vec2  texture_point = vec2(alpha / (PI * 2.0), (PI - beta) / PI);\n"
        "        return getBlockPixel(0u, texture_point);\n"
        "    }\n"
        "}\n"
        "\n"
        "rgb renderPoint(vec2 screen_point)\n"
        "{\n"
        "    vec2  screen_coord = vec2((2.0 * screen_point.x) / winsizes.x - 1.0, -(2.0 * screen_point.y) / winsizes.y + 1.0);\n"
        "    Ray   ray = CameraMakeRay(screen_coord);\n"
        "    rgb   col[RAY_DEPTH + 1u];\n"
        "    float k[RAY_DEPTH];\n"
        "    rgb   point_color[RAY_DEPTH];\n"
        "    vec3  reflected_vec;\n"
        "    intersection.max_dist = 1000000.0;\n"
        "    %s"                             /* 10 render_point_loops */
        "    clamp(col[0], vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0));\n"
        "    return col[0];\n"
        "}\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_FragColor = vec4(renderPoint(gl_FragCoord.xy), 1.0);\n"
        "}\n", ray_depth_, ray_range_, lights_.size(), MATERIALS_NUM, world_->size().x, world_->size().y, world_->size().z,
               texture_switch, lights_loop, render_point_loops);

    delete [] texture_switch;
    delete [] lights_loop;
    delete [] render_point_loops;

    return str_shader;
}

char* RayTracer::writeTextureSwitch()
{
    char* texture_switch = new char[54 * MATERIALS_NUM + 4] {};

    for (unsigned int i = 0; i < MATERIALS_NUM; ++i)
        sprintf(texture_switch + strlen(texture_switch), "case %uu: col = texture2D(textures[%uu], point); break;\n", i, i);

    return texture_switch;
}

char* RayTracer::writeLightsLoop() const
{
    char* lights_loop = new char[40 * lights_.size() + 4] {};

    for (unsigned int i = 0; i < lights_.size(); ++i)
        sprintf(lights_loop + strlen(lights_loop), "enlighten(%uu, specular, diffused);\n", i);

    return lights_loop;
}

char* RayTracer::writeRenderPointLoop() const
{
    char* render_point_loops = new char[250 * ray_depth_ + 4] {};

    for (unsigned int i = 0; i < ray_depth_; ++i)
        sprintf(render_point_loops + strlen(render_point_loops),
            "intersection.ray = ray;\n"
            "col[%uu] = RayTrace(point_color[%uu], k[%uu]);\n"
            "reflected_vec = reflect(ray.direction, intersection.normal);\n"
            "ray = Ray(intersection.point, reflected_vec);\n", i, i, i);

    for (int i = static_cast<int>(ray_depth_) - 2; i >= 0; --i)
        sprintf(render_point_loops + strlen(render_point_loops), "col[%d] += col[%d + 1] * point_color[%d] * k[%d];\n", i, i, i, i);

    return render_point_loops;
}

void RayTracer::setCamera()
{
    shader_.setUniform("camera.sizes",    sf::Glsl::Vec2(camera_->sizes_.x,       camera_->sizes_.y));
    shader_.setUniform("camera.position", sf::Glsl::Vec3(camera_->position_.x,    camera_->position_.y,    camera_->position_.z));
    shader_.setUniform("camera.forward",  sf::Glsl::Vec3(camera_->getForward().x, camera_->getForward().y, camera_->getForward().z));
    shader_.setUniform("camera.right",    sf::Glsl::Vec3(camera_->getRight().x,   camera_->getRight().y,   camera_->getRight().z));
    shader_.setUniform("camera.up",       sf::Glsl::Vec3(camera_->getUp().x,      camera_->getUp().y,      camera_->getUp().z));
}

void RayTracer::setScene()
{
    char* var = new char[64] {};

    for (unsigned int i = 0; i < lights_.size(); ++i)
    {
        sprintf(var, "lights[%d].position", i);
        shader_.setUniform(var, sf::Glsl::Vec3(lights_[i].position.x, lights_[i].position.y, lights_[i].position.z));

        sprintf(var, "lights[%d].color", i);
        shader_.setUniform(var, sf::Glsl::Vec3(lights_[i].color.x, lights_[i].color.y, lights_[i].color.z));

        sprintf(var, "lights[%d].diffused_intensity", i);
        shader_.setUniform(var, lights_[i].diffused_intensity);

        sprintf(var, "lights[%d].specular_intensity", i);
        shader_.setUniform(var, lights_[i].specular_intensity);
    }

    shader_.setUniform("world", world_->getMap());
    shader_.setUniform("mapsizes", sf::Glsl::Ivec2(static_cast<int>(world_->getMapSize().x),
                                                   static_cast<int>(world_->getMapSize().y)));

    delete [] var;
}

void RayTracer::setMaterials()
{
    char* var = new char[64] {};

    for (unsigned int i = 0; i < MATERIALS_NUM; ++i)
    {
        sprintf(var, "materials[%d].brightness", i);
        shader_.setUniform(var, MATERIALS[i].brightness);

        sprintf(var, "materials[%d].reflection", i);
        shader_.setUniform(var, MATERIALS[i].reflection);

        sprintf(var, "materials[%d].flare_size", i);
        shader_.setUniform(var, MATERIALS[i].flare_size);

        sprintf(var, "textures[%d]", i);
        shader_.setUniform(var, MATERIALS[i].texture);
    }

    delete[] var;
}

} // namespace puza