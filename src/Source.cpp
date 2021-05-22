#include "game.h"

#include <iostream>



Game* game = nullptr;
int main() 
{

	game = new Game(1920, 1080, "Game");

	float deltaTime = 0.0f;
	float oldTime = 0.0f;

	game->init();

    // Main loop
	while (!glfwWindowShouldClose(game->getWindow())) {
        float newTime = glfwGetTime();
        deltaTime = newTime - oldTime;
		if (deltaTime > 0.1f) { deltaTime = 0.1f; }
        oldTime = newTime;
		game->handleEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glClearColor(0.180f-0.1f, 0.192f - 0.1f, 0.192f - 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		game->render();
		game->renderImgui(deltaTime);

		glfwSwapBuffers(game->getWindow());
		glfwPollEvents();
	}
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(game->getWindow());
    glfwTerminate();
	delete game;
}