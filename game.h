#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "./imgui/imgui.h"
#include "./imgui/imgui_impl_glfw.h"
#include "./imgui/imgui_impl_opengl3.h"

#include <iostream>
#include <vector>
#include <cmath>

#include "./logic/resourceManager.h"
#include "./logic/spriteRenderer.h"
#include "./components/worldObject.h"
#include "./logic/physics.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
class Game
{
private:
	float dt;
	float timePassed = 0.0f; // In days
	float renderSpeed = 0.0f;
	float dtMovement;

	SpriteRenderer *renderer = nullptr;
	GLFWwindow *window = nullptr;
	physics *bodyPhysics;

	float scrWidth, scrHeight;
	float speed = 1000.0f;
	glm::vec3 rotation = glm::vec3(0.0f, glm::radians(180.0f), 0.0f);
	//environment
	std::vector<worldObject *> worldObjects;

	float planetScale = 1.0f;
	//New body
	float bodyVelocity = 100000.0f;
	float bodyMass = 1000.0f;
	float bodyRadius = 10000000.0f;
	glm::vec4 bodyColor = glm::vec4(0.5f, 0.7f, 0.4f, 1.0f);

public:
	//constructor & destructor
	Game(const unsigned int SCR_WIDTH, const unsigned int SCR_HEIGHT, const char *name);
	~Game();
	//Inititialise Game state
	void init();
	//--------imgui--------
	void renderImgui(float dt);
	//Gameloop
	void handleEvents();
	void render();
	//getset
	GLFWwindow *getWindow();
};