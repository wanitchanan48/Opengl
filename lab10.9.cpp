// lab10.9.cpp
// Computer Graphics: Lab10.09 - Spotlight Pyramid

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_m.h"
#include "camera.h"

#include <iostream>

// function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,"Spotlight Pyramid", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // resize callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // --------------------------------
    // OpenGL settings
    // --------------------------------
    glEnable(GL_DEPTH_TEST);
    Shader lightingShader("colors.vs", "colors.fs");
    Shader lightCubeShader("cube.vs", "cube.fs");
    // --------------------------------
    // Pyramid vertices
    // position + normal + texture
    // --------------------------------
    float vertices[] = 
    {
        -0.5f, -0.5f,  0.5f,
         0.0f,  0.447f,  0.894f,
         0.0f, 0.0f,

         0.5f, -0.5f,  0.5f,
         0.0f,  0.447f,  0.894f,
         1.0f, 0.0f,

         0.0f,  0.5f,  0.0f,
         0.0f,  0.447f,  0.894f,
         0.5f, 1.0f,

         // =========================
         // Right
         // =========================

          0.5f, -0.5f,  0.5f,
          0.894f, 0.447f, 0.0f,
          0.0f, 0.0f,

          0.5f, -0.5f, -0.5f,
          0.894f, 0.447f, 0.0f,
          1.0f, 0.0f,

          0.0f,  0.5f,  0.0f,
          0.894f, 0.447f, 0.0f,
          0.5f, 1.0f,

          // =========================
          // Back
          // =========================

           0.5f, -0.5f, -0.5f,
           0.0f, 0.447f, -0.894f,
           0.0f, 0.0f,

          -0.5f, -0.5f, -0.5f,
           0.0f, 0.447f, -0.894f,
           1.0f, 0.0f,

           0.0f,  0.5f,  0.0f,
           0.0f, 0.447f, -0.894f,
           0.5f, 1.0f,
            // =========================
           // Left
           // =========================

           -0.5f, -0.5f, -0.5f,
           -0.894f, 0.447f, 0.0f,
            0.0f, 0.0f,

           -0.5f, -0.5f,  0.5f,
           -0.894f, 0.447f, 0.0f,
            1.0f, 0.0f,

            0.0f,  0.5f,  0.0f,
           -0.894f, 0.447f, 0.0f,
            0.5f, 1.0f,


            // =========================
            // Bottom
            // =========================

            -0.5f, -0.5f,  0.5f,
             0.0f, -1.0f, 0.0f,
             0.0f, 0.0f,

             0.5f, -0.5f, -0.5f,
             0.0f, -1.0f, 0.0f,
             1.0f, 1.0f,

             0.5f, -0.5f,  0.5f,
             0.0f, -1.0f, 0.0f,
             1.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,
             0.0f, -1.0f, 0.0f,
             0.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,
             0.0f, -1.0f, 0.0f,
             0.0f, 1.0f,

             0.5f, -0.5f, -0.5f,
             0.0f, -1.0f, 0.0f,
             1.0f, 1.0f
    };
    // Pyramid positions

    glm::vec3 pyramidPositions[] = {
       glm::vec3(0.0f,  0.0f,  0.0f),
       glm::vec3(2.0f,  1.0f, -3.0f),
       glm::vec3(-2.0f, 1.0f, -3.0f),
       glm::vec3(0.0f,  1.0f, -6.0f),
       glm::vec3(3.0f,  0.0f, -7.0f),
       glm::vec3(-3.0f, 0.0f, -7.0f),
       glm::vec3(0.0f,  2.0f, -9.0f),
       glm::vec3(3.0f,  2.0f, -11.0f),
       glm::vec3(-3.0f, 2.0f, -11.0f),
       glm::vec3(0.0f,  0.0f, -13.0f)
    };
    unsigned int VBO;
    unsigned int pyramidVAO;

    glGenVertexArrays(1, &pyramidVAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(pyramidVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture coordinate

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    unsigned int diffuseMap = loadTexture("Brick 159.jpg");
    unsigned int specularMap = loadTexture("Brick 159.jpg");
    lightingShader.use();

    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT);

        lightingShader.use();
// ตำแหน่งของ Spotlight
        lightingShader.setVec3("light.position", camera.Position);
// ทิศทาง Spotlight
// // เปลี่ยนค่าตรงนี้เพื่อหันซ้าย/ขวา
        lightingShader.setVec3("light.direction", glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f)));
   // มุม Spotlight
        lightingShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
// ตำแหน่งกล้อง
        lightingShader.setVec3("viewPos", camera.Position);
        // --------------------------------
       // Light properties
       // --------------------------------

        lightingShader.setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
        lightingShader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        lightingShader.setFloat("light.constant", 1.0f);
        lightingShader.setFloat("light.linear", 0.09f);
        lightingShader.setFloat("light.quadratic", 0.032f);
        // --------------------------------
       // Material
       // --------------------------------

        lightingShader.setFloat("material.shininess", 32.0f);
          glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
          glm::mat4 view = camera.GetViewMatrix();
          lightingShader.setMat4("projection", projection);
          lightingShader.setMat4("view", view);
          glActiveTexture(GL_TEXTURE0);
          glBindTexture(GL_TEXTURE_2D, diffuseMap);
          glActiveTexture(GL_TEXTURE1);
          glBindTexture(GL_TEXTURE_2D, specularMap);

          glBindVertexArray(pyramidVAO);

          for (unsigned int i = 0; i < 10; i++)
          {
              glm::mat4 model = glm::mat4(1.0f);
              model = glm::translate(model, pyramidPositions[i]);
          float angle = 20.0f * i;   
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                lightingShader.setMat4("model", model);
// Pyramid มี 18 vertices
                glDrawArrays(GL_TRIANGLES, 0, 18);
          }

          // --------------------------------
          // Swap buffers
          // --------------------------------

          glfwSwapBuffers(window);
          glfwPollEvents();
    }

    // --------------------------------
    // Delete resources
    // --------------------------------

    glDeleteVertexArrays(1, &pyramidVAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();return 0;
}
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// ========================================
// Resize window
// ========================================

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}              
             
                   