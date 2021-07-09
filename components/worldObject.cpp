#include "worldObject.h"
worldObject::worldObject(glm::vec3 Pos, glm::vec3 Velocity, float Mass, float Radius, glm::vec4 Color, glm::vec3 Rotation)
	: position(Pos), velocity(Velocity), radius(Radius), color(Color), rotation(Rotation)
{
	setMass(Mass);
	// float xyzSize = cbrt(mass / 5510.0f);
	size = glm::vec3(radius, radius, radius);
}
worldObject::~worldObject()
{
}
void worldObject::drawWorldObject(SpriteRenderer &renderer, float dt)
{
	velocity += acceleration * (dt * 3600 * 24);
	position += velocity * (dt * 3600 * 24);
	renderer.DrawSprite(color, position, size, rotation);
	trace->addVertex(position, color, dt);
	trace->drawTrace();
}
void worldObject::setMass(float nMass)
{
	this->mass = nMass;
	if (color == glm::vec4(0.0f, 0.0f, 0.0f, 0.0f))
	{
		float massColor = std::log10(mass / std::pow(10, 20)) / 10;
		this->color = glm::vec4(massColor, std::abs(1.0f - massColor * 2), massColor / 2, 1.0f);
	}
}