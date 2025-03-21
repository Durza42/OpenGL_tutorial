#ifndef LIGHT_H
#define LIGHT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "Cube.h"
#include "Light_spec.h"
#include "Texture_spec.h"

/**
 * @brief The light source 
 */
class Light : public Cube
{
public:

    Light(bool delay_init = false);
    ~Light();
    void init();

    struct Instance : _Instance
    {
        using _Instance::_Instance;

        Light_spec spec = SUN_LIGHT;
        int iLight = 0;

        const Light_spec get_spec() const { return spec; }
        void set_spec(Light_spec _spec);
        void prepare_draw();
    };

private:

    std::vector<Instance> instances;
    using Cube::vertices;
    using Cube::get_vertices;

public:

    Instance& create(const Shader& shd);
};

#endif