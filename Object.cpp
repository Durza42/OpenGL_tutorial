#include "Object.h"
#include <span> // Object::buffer

/**
 * @brief Construct a new object by initializing the VBO, the VAO and the EBO
 */
Object::Object(bool delay_init /* = false */)
{
    if(not delay_init)
        init();
}

/**
 * @brief Destroy the object by freeing its VAO, its VBO and its EBO (only if the object is loaded)
 */
Object::~Object()
{
    if(loaded)
        destroy();
    loaded = false;
}

/**
 * @brief initializes the VBO, the VAO, abd buffers the vertices
 */
void Object::init()
{
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);  

    std::cout << "from Object\n";

    buffer();

    loaded = true;
}

/**
 * @brief frees the allocated datas
 */
void Object::destroy()
{
    if(not loaded)
        return;

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    loaded = false;
}

/**
 * @brief check if the object had been loaded
 * @return true if the object had been loaded
 * @return false if not
 */
bool Object::is_loaded() const
{
    return loaded;
}

/**
 * @brief buffers the vertices into the curently bound VBO in static mod
 */
void Object::buffer()
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
        std::span<float, NB_TO_DRAW> raw_vertices { get_vertices().data(), get_vertices().size() };

        // vertices --> VBO ; remembered by the VAO
        // GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
        // GL_STATIC_DRAW: the data is set only once and used many times.
        // GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
        glBufferData(GL_ARRAY_BUFFER, raw_vertices.size_bytes(), raw_vertices.data(), GL_STATIC_DRAW);

        // how datas are organised
        // first argument is cuz : layout (position = 0)
        // second argument cuz we deal with vec3
        // third cuz vec3 have float values
        // fourth : coordinates are NOT normalized
        // fifth : size between two consecutive vec3 begining (here we have only one vec3 (the color of the vertice) between two beginings)
        // sixth : data offset
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, NB_DATA_PER_VERTICE * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // same for texture coordinates
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, NB_DATA_PER_VERTICE * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // same for texture coordinates
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, NB_DATA_PER_VERTICE * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


/**
 * @brief draws an Object
 * @param to_draw the parameters of the instance to draw (eg world position, shader, ...)
 */
void Object::draw(_Instance& to_draw)
{
    to_draw.shader.activate(to_draw.texture);

    glBindVertexArray(VAO);

    to_draw.shader.set_model(to_draw.get_model_mat4());
    if(to_draw.type != LIGHT)
    {
        to_draw.shader.set_model_normals(
            glm::mat3(glm::transpose(glm::inverse(to_draw.get_model_mat4())))
        );
    }

    to_draw.prepare_draw();

    glDrawArrays(GL_TRIANGLES, 0, NB_TO_DRAW);

    glBindVertexArray(0);

    to_draw.shader.desactivate(to_draw.texture);
}
