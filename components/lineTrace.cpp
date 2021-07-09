#include "lineTrace.h"

LineTrace::LineTrace()
{
    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &this->VBO);

    glBindVertexArray(this->quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
void LineTrace::addVertex(glm::vec3 pos, glm::vec4 color, float dt)
{
    float universeScale = 8.0f * std::pow(10, 8);
    this->timePassed += dt;
    if (this->timePassed >= 0.05f)
    {
        this->vertices.push_back(pos.x / universeScale);
        this->vertices.push_back(pos.y / universeScale);
        this->vertices.push_back(pos.z / universeScale);
        this->timePassed = 0.0f;
        if (this->vertices.size() >= 6000)
            vertices.erase(vertices.begin(), vertices.begin() + 3);
    }
}
void LineTrace::drawTrace()
{

    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), &this->vertices[0], GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    Shader shader = ResourceManager::GetShader("shader");
    shader.Use();
    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    shader.SetMatrix4("model", model);
    glDrawArrays(GL_LINE_STRIP,
                 0,
                 this->vertices.size() / 3);
    glBindVertexArray(0);
}
