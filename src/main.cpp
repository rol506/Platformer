#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include <chrono>
#include <memory>
#include <cstring>

#include "Renderer/ShaderProgram.h"
#include "Renderer/Texture2D.h"
#include "Renderer/Sprite2D.h"
#include "Renderer/Renderer.h"

#include "Resources/ResourceManager.h"

#include "Physics/PhysicsEngine.h"

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::ivec2 gWindowSize(640, 360); //640x360
static void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    gWindowSize.x = width;
    gWindowSize.y = height;

    RenderEngine::Renderer::setVieport(gWindowSize.x, gWindowSize.y, 0, 0);
}

int main(int argc, char** argv)
{
    bool displayFPS = true;

    ResourceManager::setExecutablePath(argv[0]);
    for (int i = 0; i < argc; ++i)
    {
        if (strcmp(argv[i], "--NOFPS") == 0)
        {
            displayFPS = false;
        }
    }

    /* Initialize the library */
    if (!glfwInit())
    {
        std::cerr << "glfwInit failed!\n";
        return -1;
    }

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(gWindowSize.x, gWindowSize.y, "Platformer", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create window!\n";
        glfwTerminate();
        return -1;
    }

    //glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGL())
    {
        std::cerr << "Failed to load GLaD!\n";
        glfwTerminate();
        return -1;
    }

    std::cout << "Renderer: " << RenderEngine::Renderer::getRendererStr() << "\n";
    std::cout << "OpenGL version: " << RenderEngine::Renderer::getVersionStr() << "\n";

    {
        auto shader = ResourceManager::loadShaders("SpriteShader", "res/shaders/vSprite.glsl", "res/shaders/fSprite.glsl");
        auto texture = ResourceManager::loadTexture("PlayerTexture", "res/textures/lol.png", GL_LINEAR, GL_REPEAT);
        auto player = ResourceManager::loadSprite("Player", "SpriteShader", "PlayerTexture");
        auto wall = ResourceManager::loadSprite("Wall", "SpriteShader", "PlayerTexture");

        wall->setTargetPosition(glm::vec2(gWindowSize.x / 2, gWindowSize.y / 2));

        glm::mat4 projection = glm::ortho(0.0f, 1.0f * gWindowSize.x, 0.0f, 1.0f * gWindowSize.y, -10.0f, 10.0f);
        shader->use();
        shader->setMat4(projection, "projectionMatrix");

        glm::vec2 playerPosition(100,50);
        player->setPosition(playerPosition);
        player->setTargetPosition(playerPosition);

        int outline = 0;

        //for FPS counter
        double lastTime = glfwGetTime();
        int nbFrames = 0;

        //timing
        auto frameLastTime = std::chrono::high_resolution_clock::now();
        double frameDeltaTime = 0.f;

        PhysicsEngine::init();
        PhysicsEngine::addDynamicGameObject(player);
        PhysicsEngine::addDynamicGameObject(wall);

        RenderEngine::Renderer::setClearColor(0.5f, 1.0f, 1.0f, 1.0f);
        RenderEngine::Renderer::setDepthTest(true);
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            //timing
            auto timeNow = std::chrono::high_resolution_clock::now();
            frameDeltaTime = std::chrono::duration<double, std::milli>(timeNow - frameLastTime).count();
            frameLastTime = timeNow;

            //FPS counter
            double currentTime = glfwGetTime();
            if (currentTime - lastTime >= 1.0)
            {
                //print fps and reset counter
                if (displayFPS)
                    std::cout << "FPS: " << nbFrames << "\n";
                nbFrames = 0;
                lastTime += 1.0f;
            }

#pragma region physicsAndInput

            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true); //close when escape pressed

            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) player->addVelocity(glm::vec2(0.f, 0.5f));
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) player->addVelocity(glm::vec2(0.f, -0.5f));
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) player->addVelocity(glm::vec2(-0.5f, 0.f));
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) player->addVelocity(glm::vec2(0.5f, 0.f));

            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            ypos = -(ypos - gWindowSize.y); //invert y position

            auto rayCastHit = PhysicsEngine::mouseRayCast(xpos, ypos);
            if (rayCastHit)
            {
                shader->use();
                shader->setInt(1, "outline");
            }
            else {
                shader->use();
                shader->setInt(0, "outline");
            }

            PhysicsEngine::update(frameDeltaTime);

#pragma endregion

#pragma region render

            nbFrames++;

            RenderEngine::Renderer::clear();

            player->render(player->getPosition(), player->getSize(), 0, 0);

            //draw another rect
            wall->render(wall->getPosition(), wall->getSize(), 0, 0);


            /* Swap front and back buffers */
            glfwSwapBuffers(window);

#pragma endregion

            /* Poll for and process events */
            glfwPollEvents();
        }

        PhysicsEngine::terminate();
        ResourceManager::unloadAllResources();
        glfwTerminate();
        return 0;
    }
}