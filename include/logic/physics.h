#pragma once
#include <vector>
#include <cmath>

#include "worldObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class physics
{
private:
	const float gravitationalConst = 6.67408 * 0.00000000001 /*for tons + km*/  /**/;
	float distance;
	glm::vec3 direction;
public:
	physics();
	void computeVel(std::vector<worldObject*> &worldObjects, float dt, float& renderingSpeed);
};