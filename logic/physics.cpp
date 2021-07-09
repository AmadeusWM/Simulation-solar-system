#include "physics.h"
physics::physics()
{
}
void physics::computeVel(std::vector<worldObject *> &worldObjects, float dt, float &renderSpeed)
{
	dt *= (3600 * 24); // convert time from days to seconds
	std::vector<int> removed = {};
	for (int i = 0; i < worldObjects.size(); i++)
	{ //iterate through all bodies present
		worldObject *body1 = worldObjects.at(i);
		body1->acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
		for (int j = 0; j < worldObjects.size(); j++)
		{ //iterate through all other bodies present
			if (i != j)
			{
				worldObject *body2 = worldObjects.at(j);
				//calculate the normalized direction of the body
				direction = glm::normalize(body2->position - body1->position);
				//calculate the distance between body i and body j
				distance = glm::distance(body2->position, body1->position);
				if (distance <= body2->radius + body1->radius && body1->mass >= body2->mass)
				{
					glm::vec3 momentum = body1->velocity * body1->mass + body2->velocity * body2->mass;
					body1->setMass(body1->mass + body2->mass);
					body1->velocity = momentum / body1->mass;
					body1->radius += body2->radius;
					body1->size += body2->radius;
					std::cout << "the second number is going to be erased" << i << " " << j << "\n";

					removed.push_back(j);
				}
				else
				{
					//calculate and add the force (converted to acceleration) that body j causes upon body i
					body1->acceleration += direction * (gravitationalConst * (body2->mass) / (distance * distance));
				}
			}
		}
	}
	for (int i = removed.size()-1; i >= 0 ; i--)
	{
		worldObjects.erase(worldObjects.begin() + removed[i]);
	}
}