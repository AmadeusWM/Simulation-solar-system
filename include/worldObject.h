#pragma once

#include "spriteRenderer.h"
#include "resourceManager.h"
#include "lineTrace.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <cmath>

class worldObject
{
public:
	LineTrace *trace = new LineTrace();

	float mass, radius;
	glm::vec3 position, size, velocity, rotation, acceleration;
	glm::vec4 color;

	worldObject(glm::vec3 Pos,
				glm::vec3 Velocity = glm::vec3(0.0f, 0.0f, 0.0f),
				float Mass = 1000.0f,
				float Radius = 1000.0f,
				glm::vec4 Color = glm::vec4(0.0f,0.0f,0.0f,0.0f),
				glm::vec3 Rotation = glm::vec3(0.0f, 0.0f, 0.0f));
	~worldObject();

	void drawWorldObject(SpriteRenderer &renderer, float dt);
	void setMass(float mass);
};