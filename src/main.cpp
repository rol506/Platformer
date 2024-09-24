#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include <chrono>
#include <memory>

#include "Renderer/ShaderProgram.h"
#include "Renderer/Texture2D.h"
#include "Resources/ResourceManager.h"

int g_windowSizeX = 640;
int g_windowSizeY = 360;

GLfloat vertecies[] = {
    // X      Y     Z     R     G     B     TX    TY
     -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,      //    1 --- 4
     -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,      //    | \   |
      0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,      //    |   \ |
      0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f       //    2 --- 3
};

unsigned int elements[] = {
    0, 1, 2,
    0, 2, 3
};

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_windowSizeX = width;
    g_windowSizeY = height;

    glViewport(0, 0, g_windowSizeX, g_windowSizeY);
}

int main(int argc, char** argv)
{
    ResourceManager::setExecutablePath(argv[0]);

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
    GLFWwindow* window = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "Hello World", NULL, NULL);
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

        GLuint VBO, EBO, VAO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertecies), vertecies, GL_STATIC_DRAW);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        auto program = ResourceManager::loadShaders("DefaultShader", "res/shaders/vertex.glsl", "res/shaders/fragment.glsl");
        auto texture = ResourceManager::loadTexture("DefaultTexture", "res/textures/texture.jpg", GL_NEAREST, GL_REPEAT);

        float timer = 0.f;

        //for FPS counter
        double lastTime = glfwGetTime();
        int nbFrames = 0;

        //timing
        auto frameLastTime = std::chrono::high_resolution_clock::now();
        double frameDeltaTime = 0.f;

        program->use();
        program->setInt(0, "tex");
        
        glClearColor(0.5f, 1.0f, 1.0f, 1.0f);
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
                std::cout << "FPS: " << nbFrames << "\n";
                nbFrames = 0;
                lastTime += 1.0f;
            }

#pragma region physicsAndInput

            timer = sin(glfwGetTime()) / 2.0f + 0.5f;

            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true); //close when escape pressed

#pragma endregion

#pragma region render

            nbFrames++;

            program->use();
            program->setFloat(timer, "timer");

            glClear(GL_COLOR_BUFFER_BIT);

            glBindVertexArray(VAO);
            texture->bind();
            //program.use(); //you should use this but I will not because I used it earlier
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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