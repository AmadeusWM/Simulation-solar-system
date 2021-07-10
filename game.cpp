#include "game.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
//mouse movement
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
// key inputs
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
//positions camera    (eye = camera pos, center = where we look at, up = camera upside)    Right hand technique
glm::vec3 eye = glm::vec3(0.0f, -2613.0f, 0.0f);
glm::vec3 center = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);

bool altPressed = 0;

bool firstMouse = true;
float lastX = 400, lastY = 300;
float yaw = 90.0f;
float pitch = 0.0f;

float xoffset;
float yoffset;

bool newBody = 0;
glm::vec3 calcPos(float RG, float IN, float TA, float W, float OM)
{
    glm::vec3 pos = glm::vec3(RG, 0.0f, 0.0f);
    pos = glm::rotateZ(pos, glm::radians(TA + W));
    pos = glm::rotateY(pos, glm::radians(-IN));
    pos = glm::rotateZ(pos, glm::radians(OM));
    return pos;
}
glm::vec3 calcVel(float orbitalVel, float IN, float TA, float W, float OM)
{
    glm::vec3 velocity = glm::vec3(0.0f, orbitalVel, 0.0f);
    velocity = glm::rotateZ(velocity, glm::radians(TA + W));
    velocity = glm::rotateY(velocity, glm::radians(-IN));
    velocity = glm::rotateZ(velocity, glm::radians(OM));
    return velocity;
}

//constructor & destructor
Game::Game(const unsigned int SCR_WIDTH, const unsigned int SCR_HEIGHT, const char *name)
    : scrWidth(SCR_WIDTH), scrHeight(SCR_HEIGHT)
{
    dt = 0.0f;

    //initialise window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //create window
    window = glfwCreateWindow(scrWidth, scrHeight, name, NULL, NULL);
    glfwMakeContextCurrent(window);

    //set callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    //keys
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glEnable(GL_DEPTH_TEST);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Show lines
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((char *)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
}
Game::~Game()
{
}
//Game state
void Game::init()
{
    //----------testing-----------

    //----------testing-----------
    //create shader object
    Shader shader = ResourceManager::LoadShader("shaders/shader.vert", "shaders/shader.frag", nullptr, "shader");
    //perspective projection
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.001f, 30000.0f);
    shader.Use();
    glUniform1i(glGetUniformLocation(ResourceManager::GetShader("shader").ID, "image"), 0);
    glUniformMatrix4fv(glGetUniformLocation(ResourceManager::GetShader("shader").ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    renderer = new SpriteRenderer(shader);
    bodyPhysics = new physics();
    //Create world objects
    worldObjects.push_back(new worldObject( //Sun
        glm::vec3(0.0f, 0.0f, 0.0f),        //position
        glm::vec3(0.0f, 0.0f, 0.0f),        //velocity
        1.98850f * std::pow(10, 30),        //mass
        695700.0f * 1000.0f));              //radius
    worldObjects.push_back(new worldObject( //Mercury
        calcPos(64679134600.0f, 7.00368501407854f, 226.121888540079f, 29.1866190252741f, 48.30463433f),
        calcVel(42565.3389f, 7.00368501407854f, 226.121888540079f, 29.1866190252741f, 48.30463433f),
        3.30104f * std::pow(10, 23),
        2439.7f * 1000,
        glm::vec4(0.663, 0.663f, 0.663f, 1.0f)));
    worldObjects.push_back(new worldObject( //Venus
        calcPos(108333091860.0f, 3.3945398907f, 100.18640864f, 55.08081360274f, 76.62280261f),
        calcVel(34978.7082f, 3.3945398907f, 100.18640864f, 55.08081360274f, 76.62280261f),
        4.86732f * std::pow(10, 24),
        6051.8f * 1000,
        glm::vec4(0.819f, 0.494f, 0.094f, 1.0f)));
    worldObjects.push_back(new worldObject( //Earth
        calcPos(147094329630.0f, 0.00286565558804565f, 357.921544278592f, 254.381064202347f, 208.193256922f),
        calcVel(30273.8213f, 0.00286565558804565f, 357.921544278592f, 254.381064202347f, 208.193256922f),
        5.97219f * std::pow(10, 24),
        6371.0f * 1000,
        glm::vec4(0.263f, 0.980f, 0.263f, 1.0f)));
    worldObjects.push_back(new worldObject( //Mars
        calcPos(225796166460.0f, 1.847921263f, 89.5830872f, 286.64009008f, 49.49399237f),
        calcVel(24355.9617f, 1.847921263f, 89.5830872f, 286.64009008f, 49.49399237f),
        6.41693f * std::pow(10, 23),
        3389.5f * 1000,
        glm::vec4(1.0f, 0.663f, 0.376f, 1.0f)));
    worldObjects.push_back(new worldObject( //Jupiter
        calcPos(762307772830.0f, 1.3038798526f, 292.4013046f, 273.7326347f, 100.523202018f),
        calcVel(13328.3755f, 1.3038798526f, 292.4013046f, 273.7326347f, 100.523202018f),
        1.89813f * std::pow(10, 27),
        69911.0f * 1000,
        glm::vec4(0.71f, 0.545f, 0.306f, 1.0f)));
    glm::vec3 saturnVel = calcVel(9223.6732f, 2.4896989636f, 213.43479455f, 336.316197359f, 113.608909179);
    std::cout << "[" << saturnVel.x << ", " << saturnVel.y << ", " << saturnVel.z << "]\n";
    worldObjects.push_back(new worldObject( //Saturn
        calcPos(1494036248690.0f, 2.4896989636f, 213.43479455f, 336.316197359f, 113.608909179),
        calcVel(9223.6732f, 2.4896989636f, 213.43479455f, 336.316197359f, 113.608909179),
        5.68319f * std::pow(10, 26),
        58232.0f * 1000,
        glm::vec4(0.749f, 0.635f, 0.365f, 1.0f)));
    worldObjects.push_back(new worldObject( //Ur mom
        calcPos(2957873288570.0f, 0.7699211577f, 227.57502956f, 97.3757466f, 74.117481227f),
        calcVel(6599.0839f, 0.7699211577f, 227.57502956f, 97.3757466f, 74.117481227f),
        8.68103f * std::pow(10, 25),
        25362.0f * 1000,
        glm::vec4(0.62f, 0.933f, 1.0f, 1.0f)));
    worldObjects.push_back(new worldObject( //Neptune
        calcPos(4581774199480.0f, 1.7643733644f, 332.86198157f, 245.45411486f, 131.62708383f),
        calcVel(5349.6763f, 1.7643733644f, 332.86198157f, 245.45411486f, 131.62708383f),
        1.0241f * std::pow(10, 26),
        24622.0f * 1000,
        glm::vec4(0.212f, 0.341f, 0.875f, 1.0f)));
    worldObjects.push_back(new worldObject( //Pluto
        calcPos(5115017966500.0f, 17.1086387326386f, 66.7532148f, 117.41919597933f, 110.299540324f),
        calcVel(5532.1882f, 17.1086387326386f, 66.7532148f, 117.41919597933f, 110.299540324f),
        1.30f * std::pow(10, 22),
        1188.0f * 1000,
        glm::vec4(0.89f, 0.847f, 0.745f, 1.0f)));
    float moonDistance = 147094329630.0f + 384402000.0f;
    float moonVelocity = 30273.8213f + 1024.0f;
    worldObjects.push_back(new worldObject( //random spaceship
        calcPos(moonDistance, 0.00286565558804565f, 357.921544278592f, 254.381064202347f, 208.193256922f),
        calcVel(moonVelocity, 0.00286565558804565f, 357.921544278592f, 254.381064202347f, 208.193256922f),
        7.34767309f * std::pow(10, 22),
        1737.0f * 1000.0f,
        glm::vec4(0.563f, 0.50f, 0.963f, 1.0f)));
    // srand(time(NULL));
    // for (int i = 1; i < 200; i++)
    // {
    //     float randomFloatX = rand() / (RAND_MAX + 1.0f);
    //     float randomFloatY = rand() / (RAND_MAX + 1.0f);
    //     float randomFloatZ = rand() / (RAND_MAX + 1.0f);
    //     std::cout << randomFloatX << std::endl;
    //     worldObjects.push_back(new worldObject(                                                                           //Sun
    //         glm::vec3(5000000000000.0f * randomFloatX, 5000000000000.0f * randomFloatY, randomFloatZ * 5000000000000.0f), //position
    //         glm::vec3(0.0f, 0.0f, 0.0f),                                                                                  //velocity
    //         1.98850f * std::pow(10, 30),                                                                                  //mass
    //         695700.0f * 2000.0f,
    //         glm::vec4(randomFloatX, randomFloatY, randomFloatZ, 1.0f)));
    // }
}
//--------imgui--------
int selectedPlanet = 0;
void Game::renderImgui(float deltaTime)
{

    dt = 0.00025f * renderSpeed;
    dtMovement = 0.00025f;
    timePassed += dt;
    ImGui_ImplOpenGL3_NewFrame();

    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    {
        ImGui::Begin("Executions");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        const char *items[worldObjects.size()]{"Sun", "Mercury", "Venus", "Earth", "Mars", "Jupiter", "Saturn", "Uranus", "Neptune", "Pluto"};
        for (int i = 10; i < worldObjects.size(); i++){
            const char* bodyName = "uncategorized body";
            items[i] = bodyName;
        }
        ImGui::Combo("Planets", &selectedPlanet, items, IM_ARRAYSIZE(items));
        // ImGui::Text("");
        if (ImGui::TreeNode("Positions"))
        {
            ImGui::InputFloat("x: %.1f meters", &worldObjects.at(selectedPlanet)->position.x, 10000000000.0f, 200000000.0f);
            ImGui::InputFloat("y: %.1f meters", &worldObjects.at(selectedPlanet)->position.y, 10000000000.0f, 200000000.0f);
            ImGui::InputFloat("z: %.1f meters", &worldObjects.at(selectedPlanet)->position.z, 10000000000.0f, 200000000.0f);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Velocity"))
        {
            ImGui::InputFloat("x: %.1f m/s", &worldObjects.at(selectedPlanet)->velocity.x, 1000.0f, 200.0f);
            ImGui::InputFloat("y: %.1f m/s", &worldObjects.at(selectedPlanet)->velocity.y, 1000.0f, 200.0f);
            ImGui::InputFloat("z: %.1f m/s", &worldObjects.at(selectedPlanet)->velocity.z, 1000.0f, 200.0f);
            ImGui::TreePop();
        }

        ImGui::InputFloat("Mass: %.3f km", &worldObjects.at(selectedPlanet)->mass, 10000000000000.0f, 1000000000000.0f);

        ImGui::InputFloat("Render speed", &renderSpeed, 100.0f, 10.0f);
        ImGui::Text("Time passed: %.2f Days", timePassed);

        if (ImGui::Button("Toggle scale"))
        {
            if (renderer->planetSize == 8.0f * std::pow(10, 8))
            {
                renderer->planetSize = 1000000.0f;
                float radius = 695700.0f * 20.0f;
                worldObjects.at(0)->size = glm::vec3(radius, radius, radius);
            }
            else
            {
                renderer->planetSize = 8.0f * std::pow(10, 8);
                float radius = 695700.0f * 1000.0f;
                worldObjects.at(0)->size = glm::vec3(radius, radius, radius);
            }
        }
        ImGui::Spacing();
        ImGui::InputFloat("Camera movementspeed", &speed, 1000.0f, 100.0f);
        if (ImGui::TreeNode("Add a new body"))
        {
            ImGui::Text("Launch a new body with right click!");
            ImGui::InputFloat("Velocity(m/s)", &bodyVelocity, 1000.0f, 200.0f);
            ImGui::InputFloat("Mass(*10^25kg)", &bodyMass, 1000.0f, 200.0f);
            ImGui::InputFloat("Radius(m)", &bodyRadius, 1000000.0f, 200000.0f);
            ImGui::ColorEdit3("Color", &bodyColor.x);
            ImGui::TreePop();
        }
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
//Gameloop
void Game::handleEvents()
{
    //Input
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        speed = 1000.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        eye += center * dtMovement* speed;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        eye -= center * dtMovement * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        eye.x -= (center.x * cos(glm::radians(-90.0f)) - center.y * sin(glm::radians(-90.0f))) * dtMovement * speed;
        eye.y -= (center.x * sin(glm::radians(-90.0f)) + center.y * cos(glm::radians(-90.0f))) * dtMovement * speed;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        eye.x -= (center.x * cos(glm::radians(90.0f)) - center.y * sin(glm::radians(90.0f))) * dtMovement * speed;
        eye.y -= (center.x * sin(glm::radians(90.0f)) + center.y * cos(glm::radians(90.0f))) * dtMovement * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        eye.z += speed * dtMovement;
    }

    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        eye.z -= speed * dtMovement;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        altPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_RELEASE)
    {
        altPressed = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPos(window, lastX, lastY);
    }
    //Launch Planet
    if (newBody == 1)
    {
        std::cout << eye.x << " " << eye.y << " " << eye.z << " -- " << center.x << " " << center.y << " " << center.z << "\n";
        worldObjects.push_back(new worldObject( //Jupiter
            glm::vec3(eye.x * (8.0f * std::pow(10, 8)), eye.y * (8.0f * std::pow(10, 8)), eye.z * (8.0f * std::pow(10, 8))),
            center * bodyVelocity,
            bodyMass* std::pow(10, 25),
            bodyRadius,
            bodyColor));
        newBody = 0;
    }
}

void Game::render()
{
    //view matrix
    glm::mat4 view;
    view = glm::lookAt(eye, eye + center, up);

    glUniformMatrix4fv(glGetUniformLocation(ResourceManager::GetShader("shader").ID, "view"), 1, GL_FALSE, glm::value_ptr(view));

    //draw objectsglBindVertexArray(VAO);
    bodyPhysics->computeVel(worldObjects, dt, renderSpeed);
    for (int i = 0; i < worldObjects.size(); i++)
    {
        worldObjects.at(i)->drawWorldObject(*renderer, dt);
    }
    // this->renderer->DrawSprite(glm::vec4(1.0f, 0.5f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1000.0f, 1000.0f, 1000.0f), glm::vec3(0.0f, 0.0f, 0.0f));
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (altPressed == 0)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        xoffset = lastX - xpos;
        yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.05;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.z = sin(glm::radians(pitch));
        direction.y = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        center = glm::normalize(direction);
    }
}

//------CALLBACKS---------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//---------getset---------
GLFWwindow *Game::getWindow()
{
    return window;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        newBody = 1;
    }
}
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        newBody = 1;
    }
}