#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

#include <vector>

class SpriteRenderer
{
public:
    float planetSize = 8.0f * std::pow(10, 8);
    float universeScale = 8.0f * std::pow(10, 8);
    std::vector<float> vertices;       // array of 12 vertices (x,y,z)
    std::vector<unsigned int> indices; // array of 12 vertices (x,y,z)
    // Constructor (inits shaders/shapes)
    SpriteRenderer(Shader &shader);
    // Destructor
    ~SpriteRenderer();
    // Renders a defined quad textured with given sprite
    void DrawSprite(glm::vec4 color, glm::vec3 position, glm::vec3 size, glm::vec3 rotate);
    void computeSphereVertices(float stackCount, float sectorCount);
    void computeSphereIndices(float stackCount, float sectorCount);

private:
    // Render state
    Shader shader;
    GLuint quadVAO, VBO, EBO;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};