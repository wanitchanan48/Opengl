// lab10.4.cpp
// Computer Graphics: Lab10.04 - Materials (bronze) Pyramid
// No Mouse

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_m.h"
#include "camera.h"

#include <iostream>

// function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


int main()
{
    // --------------------------------
    // GLFW initialize
    // --------------------------------
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // --------------------------------
    // Create window
    // --------------------------------
    GLFWwindow* window = glfwCreateWindow(
        SCR_WIDTH,
        SCR_HEIGHT,
        "Computer Graphics: Lab10.04 - Materials Bronze Pyramid",
        NULL,
        NULL
    );

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Resize window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // --------------------------------
    // GLAD
    // --------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // --------------------------------
    // OpenGL settings
    // --------------------------------
    glEnable(GL_DEPTH_TEST);

    // --------------------------------
    // Shader
    // --------------------------------
    Shader lightingShader("colors.vs", "colors.fs");

    // --------------------------------
    // Pyramid vertices
    // position + normal
    // --------------------------------
    float vertices[] =
    {
        // Front
        -0.5f, -0.5f,  0.5f,
         0.0f,  0.447f,  0.894f,

         0.5f, -0.5f,  0.5f,
         0.0f,  0.447f,  0.894f,

         0.0f,  0.5f,  0.0f,
         0.0f,  0.447f,  0.894f,

         // Right
          0.5f, -0.5f,  0.5f,
          0.894f,  0.447f,  0.0f,

          0.5f, -0.5f, -0.5f,
          0.894f,  0.447f,  0.0f,

          0.0f,  0.5f,  0.0f,
          0.894f,  0.447f,  0.0f,

          // Back
           0.5f, -0.5f, -0.5f,
           0.0f,  0.447f, -0.894f,

          -0.5f, -0.5f, -0.5f,
           0.0f,  0.447f, -0.894f,

           0.0f,  0.5f,  0.0f,
           0.0f,  0.447f, -0.894f,

           // Left
           -0.5f, -0.5f, -0.5f,
           -0.894f,  0.447f,  0.0f,

           -0.5f, -0.5f,  0.5f,
           -0.894f,  0.447f,  0.0f,

            0.0f,  0.5f,  0.0f,
           -0.894f,  0.447f,  0.0f,

           // Bottom
           -0.5f, -0.5f,  0.5f,
            0.0f, -1.0f, 0.0f,

            0.5f, -0.5f, -0.5f,
            0.0f, -1.0f, 0.0f,

            0.5f, -0.5f,  0.5f,
            0.0f, -1.0f, 0.0f,

           -0.5f, -0.5f,  0.5f,
            0.0f, -1.0f, 0.0f,

           -0.5f, -0.5f, -0.5f,

            0.0f, -1.0f, 0.0f,

            0.5f, -0.5f, -0.5f,
            0.0f, -1.0f, 0.0f
    };

    // --------------------------------
    // VAO / VBO
    // --------------------------------
    unsigned int VBO, pyramidVAO;

    glGenVertexArrays(1, &pyramidVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(pyramidVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Normal
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    // --------------------------------
    // Render loop
    // --------------------------------
    while (!glfwWindowShouldClose(window))
    {
        // Time
        float currentFrame =
            static_cast<float>(glfwGetTime());

        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Keyboard
        processInput(window);

        // Clear screen
        glClearColor(
            0.1f,
            0.1f,
            0.1f,
            1.0f
        );

        glClear(
            GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT
        );

        // --------------------------------
        // Activate shader
        // --------------------------------
        lightingShader.use();

        // --------------------------------
        // Light position
        // --------------------------------
        glm::vec3 lightPos(
            1.2f,
            1.0f,
            2.0f
        );

        lightingShader.setVec3(
            "lightPos",
            lightPos
        );

        // Camera position
        lightingShader.setVec3(
            "viewPos",
            camera.Position
        );

        // --------------------------------
        // Light properties
        // --------------------------------
        lightingShader.setVec3(
            "light.ambient",
            0.2f,
            0.2f,
            0.2f
        );

        lightingShader.setVec3(
            "light.diffuse",
            0.5f,
            0.5f,
            0.5f
        );

        lightingShader.setVec3(
            "light.specular",
            1.0f,
            1.0f,
            1.0f
        );

        // --------------------------------
        // Bronze Material
        // --------------------------------

        lightingShader.setVec3(
            "material.ambient",
            0.2125f,
            0.1275f,
            0.0540f
        );

        lightingShader.setVec3(
            "material.diffuse",
            0.7140f,
            0.4284f,
            0.1814f
        );

        lightingShader.setVec3(
            "material.specular",
            0.3936f,
            0.2719f,
            0.1667f
        );

        lightingShader.setFloat(
            "material.shininess",
            25.6f
        );

        // --------------------------------
        // Projection
        // --------------------------------
        glm::mat4 projection =
            glm::perspective(
                glm::radians(camera.Zoom),
                (float)SCR_WIDTH /
                (float)SCR_HEIGHT,
                0.1f,
                100.0f
            );

        // View
        glm::mat4 view =
            camera.GetViewMatrix();

        // Model
        glm::mat4 model =
            glm::mat4(1.0f);

        // Send matrices
        lightingShader.setMat4(
            "projection",
            projection
        );

        lightingShader.setMat4(
            "view",
            view
        );

        lightingShader.setMat4(
            "model",
            model
        );

        // --------------------------------
        // Draw Pyramid
        // --------------------------------
        glBindVertexArray(pyramidVAO);

        // Pyramid = 18 vertices
        glDrawArrays(
            GL_TRIANGLES,
            0,
            18
        );

        // --------------------------------
        // Swap buffers
        // --------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // --------------------------------
    // Delete
    // --------------------------------
    glDeleteVertexArrays(1, &pyramidVAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
  
// ========================================
// Keyboard
// ========================================

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
   {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {

        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, deltaTime
        );
    }
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}










           
       

   