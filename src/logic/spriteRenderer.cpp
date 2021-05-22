#include "spriteRenderer.h"

SpriteRenderer::SpriteRenderer(Shader &shader)
{
    this->shader = shader;
    this->initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::DrawSprite(glm::vec4 color, glm::vec3 position, glm::vec3 size, glm::vec3 rotate)
{
    // Prepare transformations
    // float planetSize = 1000000.0f;
    this->shader.Use();
    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(position / universeScale)); // First translate
    model = glm::scale(model, glm::vec3(size / planetSize));            // Last scale

    this->shader.SetMatrix4("model", model);
    this->shader.SetVector4f("objectColor", color);

    // Render sphere
    glBindVertexArray(this->quadVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glDrawElements(
        GL_TRIANGLES,    // mode
        indices.size(),  // count
        GL_UNSIGNED_INT, // type
        (void *)0        // element array buffer offset
    );
    glBindVertexArray(0);
}
void SpriteRenderer::initRenderData()
{
    const float stackCount =    20.0f; // 2.0f;
    const float sectorCount =   20.0f; // 4.0f;
    computeSphereVertices(stackCount, sectorCount);
    computeSphereIndices(stackCount, sectorCount);

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SpriteRenderer::computeSphereVertices(float stackCount, float sectorCount)
{
    // clear memory of prev arrays
    float radius = 1.0f;
    const float PI = 3.14159265359;
    vertices.clear();

    float x, y, z, xy; // vertex position

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep; // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);      // r * cos(u)
        z = radius * sinf(stackAngle);       // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep; // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }
}
void SpriteRenderer::computeSphereIndices(float stackCount, float sectorCount)
{

    // generate CCW index list of sphere triangles
    // k1--k1+1
    // |  / |
    // | /  |
    // k2--k2+1
    std::vector<int> lineIndices;
    int k1, k2;
    for (int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1); // beginning of current stack
        k2 = k1 + sectorCount + 1;  // beginning of next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if (i != (stackCount - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }

            // store indices for lines
            // vertical lines for all stacks, k1 => k2
            lineIndices.push_back(k1);
            lineIndices.push_back(k2);
            if (i != 0) // horizontal lines except 1st stack, k1 => k+1
            {
                lineIndices.push_back(k1);
                lineIndices.push_back(k1 + 1);
            }
        }
    }
}
/*void SpriteRenderer::initRenderData()
{
    // Configure VAO/VBO

    GLfloat vertices[] = {
        -1.0f, 0.0f, 1.0f,  //upper left
        1.0f, 0.0f, -1.0f,  //lower right
        -1.0f, 0.0f, -1.0f, //lower left
        1.2f, 0.0f,  1.0f,   //upper right
        - 0.8f, 0.0f, 1.0f,  //upper left
        1.2f, 0.0f, -1.0f,  //lower right
    };
    unsigned int indices[] = {
        0,1,2,
        4,5,3
    };

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}*/