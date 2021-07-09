#pragma once

#include <glm/glm.hpp>

#include <iostream>
#include <cmath> 
#include <vector>

#include "../shaders/shader.h"
#include "../logic/resourceManager.h"

class LineTrace {
    private:
        GLuint quadVAO, VBO;
        float timePassed = 0.0f;
        std::vector<float> vertices = {}; 
    public:
        LineTrace();
        void addVertex(glm::vec3 pos, glm::vec4 color, float dt);
        void drawTrace();
};