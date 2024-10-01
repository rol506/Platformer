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
#include "Resources/ResourceManager.h"

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::ivec2 gWindowSize(640, 360); //640x360

static void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    gWindowSize.x = width;
    gWindowSize.y = height;

    glViewport(0, 0, gWindowSize.x, gWindowSize.y);
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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(gWindowSize.x, gWindowSize.y, "Hello World", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create window!\n";
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGL())
    {
        std::cerr << "Failed to load GLaD!\n";
        glfwTerminate();
        return -1;
    }

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";

    {
        auto shader = ResourceManager::loadShaders("SpriteShader", "res/shaders/vSprite.glsl", "res/shaders/fSprite.glsl");
        auto texture = ResourceManager::loadTexture("Texture", "res/textures/lol.png");
        auto sprite = ResourceManager::loadSprite("Sprite", "SpriteShader", "Texture");

        glm::vec2 copyPos(-1000000);

        glm::vec2 spritePos(gWindowSize.x/2-50, gWindowSize.y/2-50);

        float timer = 0.f;

        //for FPS counter
        double lastTime = glfwGetTime();
        int nbFrames = 0;

        //timing
        auto frameLastTime = std::chrono::high_resolution_clock::now();
        double frameDeltaTime = 0.f;

        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(gWindowSize.x), 0.0f, static_cast<float>(gWindowSize.y), -100.0f, 100.0f);

        shader->use();
        shader->setMat4(projection, "projectionMatrix");
        
        glClearColor(0.5f, 1.0f, 1.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);
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

            timer = sin(glfwGetTime()) / 2.0f + 0.5f;

            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true); //close when escape pressed

            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)  spritePos = spritePos + glm::vec2( 0.0f,  0.5f) * static_cast<float>(frameDeltaTime);
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)  spritePos = spritePos + glm::vec2( 0.0f, -0.5f) * static_cast<float>(frameDeltaTime);
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)  spritePos = spritePos + glm::vec2(-0.5f,  0.0f) * static_cast<float>(frameDeltaTime);
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)  spritePos = spritePos + glm::vec2( 0.5f,  0.0f) * static_cast<float>(frameDeltaTime);

            if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) copyPos = spritePos;

#pragma endregion

#pragma region render

            nbFrames++;

            shader->use();
            shader->setFloat(timer, "timer");

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            sprite->render(spritePos, glm::vec2(1), 0, 0);

            sprite->render(copyPos, glm::vec2(1), -1, 0);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

#pragma endregion

            /* Poll for and process events */
            glfwPollEvents();
        }

        ResourceManager::unloadAllResources();
        glfwTerminate();
        return 0;
    }
}