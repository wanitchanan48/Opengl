// lab06.2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader_s.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // -----------------------------------------
    // GLFW: initialize and configure
    // -----------------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // -----------------------------------------
    // Create GLFW window
    // -----------------------------------------
    GLFWwindow* window = glfwCreateWindow(
        SCR_WIDTH,
        SCR_HEIGHT,
        "LearnOpenGL",
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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // -----------------------------------------
    // GLAD: load OpenGL function pointers
    // -----------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // -----------------------------------------
    // Build and compile shaders
    // -----------------------------------------
    Shader ourShader("vShader.vs", "fShader.glsl");

    // -----------------------------------------
    // Set up vertex data
    // -----------------------------------------
    float vertices[] = {
        // positions          // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left
    };

    unsigned int indices[] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    // -----------------------------------------
    // Create transformation : Translate
    // -----------------------------------------





     // -----------------------------------------
    // Create VAO, VBO and EBO
    // -----------------------------------------
    unsigned int VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW
    );

    // -----------------------------------------
    // Position attribute
    // -----------------------------------------
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);

    // -----------------------------------------
    // Texture coordinate attribute
    // -----------------------------------------
    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)(3 * sizeof(float))
    );

    glEnableVertexAttribArray(1);

    // -----------------------------------------
    // Load textures
    // -----------------------------------------
    unsigned int texture1;
    unsigned int texture2;

    // =========================================
    // Texture 1 : container.jpg
    // =========================================

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // Texture wrapping
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_REPEAT
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_REPEAT
    );

    // Texture filtering
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR
    );

    int width;
    int height;
    int nrChannels;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(
        "wall.jpg",
        &width,
        &height,
        &nrChannels,
        0
    );

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load container.jpg" << std::endl;
    }

    stbi_image_free(data);

    // =========================================
    // Texture 2 : awesomeface.png
    // =========================================

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // Texture wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR
    );

    data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load awesomeface.png"
            << std::endl;
    }

    stbi_image_free(data);

    // -----------------------------------------
    // Tell OpenGL which texture unit to use
    // -----------------------------------------

    ourShader.use();

    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    // -----------------------------------------
    // Render loop
    // -----------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        // Input
        processInput(window);

        // -------------------------------------
        // Clear screen
        // -------------------------------------
        glClearColor(
            0.2f,
            0.3f,
            0.3f,
            1.0f
        );

        glClear(GL_COLOR_BUFFER_BIT);

        // -------------------------------------
        // Activate texture unit 0
        // -------------------------------------
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        // -------------------------------------
        // Activate texture unit 1
        // -------------------------------------
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // เลื่อนรูปไปทางขวาและด้านบน
        float Angle = 45;
        float cosAngle = cos(Angle);
        float sinAngle = sin(Angle);

        // Translation Matrix
        float translateMatrix[] = {
            cosAngle, 0.0f, sinAngle, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            -sinAngle, cosAngle, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

        // -----------------------------------------
   // Get matrix uniform location
   // -----------------------------------------
        ourShader.use();

        unsigned int transformLoc =
            glGetUniformLocation(ourShader.ID, "transform");

        glUniformMatrix4fv(transformLoc, 1, GL_TRUE, translateMatrix);

        // -------------------------------------
        // Bind VAO
        // -------------------------------------
        glBindVertexArray(VAO);

        // -------------------------------------
        // Draw rectangle
        // -------------------------------------
        glDrawElements(
            GL_TRIANGLES,
            6,
            GL_UNSIGNED_INT,
            0
        );

        // -------------------------------------
        // Swap buffers
        // -------------------------------------
        glfwSwapBuffers(window);

        // -------------------------------------
        // Poll events
        // -------------------------------------
        glfwPollEvents();
    }

    // -----------------------------------------
    // Delete resources
    // -----------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);

    // -----------------------------------------
    // Terminate GLFW
    // -----------------------------------------
    glfwTerminate();

    return 0;
}

// =====================================================
// Process input
// =====================================================

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

// =====================================================
// Framebuffer resize callback
// =====================================================

void framebuffer_size_callback(
    GLFWwindow* window,
    int width,
    int height)
{
    glViewport(0, 0, width, height);
}











































