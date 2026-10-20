// lab10.3.cpp
// Computer Graphics: Lab10.03 - ADS Pyramid

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_m.h"
#include "camera.h"

#include <iostream>

// Function prototypes
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

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
    // -----------------------------
    // Initialize GLFW
    // -----------------------------
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // -----------------------------
    // Create window
    // -----------------------------
    GLFWwindow* window = glfwCreateWindow(
        SCR_WIDTH,
        SCR_HEIGHT,
        "Computer Graphics: Lab10.03 - ADS Pyramid",
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

    // ใช้เฉพาะการปรับขนาดหน้าต่าง
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // ไม่ใช้ Mouse ตามโจทย์
    // glfwSetCursorPosCallback(...)
    // glfwSetScrollCallback(...)
    // glfwSetInputMode(...)

    // -----------------------------
    // Initialize GLAD
    // -----------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // -----------------------------
    // OpenGL settings
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // -----------------------------
    // Shaders
    // -----------------------------
    Shader lightingShader("colors.vs", "colors.fs");
    Shader lightCubeShader("cube.vs", "cube.fs");

    // =========================================================
    // PYRAMID VERTICES
    //
    // Position                  Normal
    // =========================================================
    float vertices[] =
    {
        // -------- FRONT --------
        // position                    normal
         0.0f,  0.5f,  0.0f,       0.0f,  0.4472f,  0.8944f,
        -0.5f, -0.5f,  0.5f,       0.0f,  0.4472f,  0.8944f,
         0.5f, -0.5f,  0.5f,       0.0f,  0.4472f,  0.8944f,

         // -------- RIGHT --------
          0.0f,  0.5f,  0.0f,       0.8944f,  0.4472f,  0.0f,
          0.5f, -0.5f,  0.5f,       0.8944f,  0.4472f,  0.0f,
          0.5f, -0.5f, -0.5f,      0.8944f,  0.4472f,  0.0f,

          // -------- BACK --------
           0.0f,  0.5f,  0.0f,       0.0f,  0.4472f, -0.8944f,
           0.5f, -0.5f, -0.5f,       0.0f,  0.4472f, -0.8944f,
          -0.5f, -0.5f, -0.5f,       0.0f,  0.4472f, -0.8944f,

          // -------- LEFT --------
           0.0f,  0.5f,  0.0f,      -0.8944f,  0.4472f,  0.0f,
          -0.5f, -0.5f, -0.5f,      -0.8944f,  0.4472f,  0.0f,
          -0.5f, -0.5f,  0.5f,      -0.8944f,  0.4472f,  0.0f,

          // -------- BASE 1 --------
          -0.5f, -0.5f,  0.5f,       0.0f, -1.0f,  0.0f,
           0.5f, -0.5f,  0.5f,       0.0f, -1.0f,  0.0f,
           0.5f, -0.5f, -0.5f,       0.0f, -1.0f,  0.0f,

           // -------- BASE 2 --------
            0.5f, -0.5f, -0.5f,       0.0f, -1.0f,  0.0f,
           -0.5f, -0.5f, -0.5f,       0.0f, -1.0f,  0.0f,
           -0.5f, -0.5f,  0.5f,       0.0f, -1.0f,  0.0f
    };

    // =========================================================
    // VAO / VBO
    // =========================================================
    unsigned int VBO;
    unsigned int pyramidVAO;

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
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)0
    );

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

    // =========================================================
    // Light VAO
    // =========================================================
    unsigned int lightCubeVAO;

    glGenVertexArrays(1, &lightCubeVAO);

    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);

    // =========================================================
    // Render loop
    // =========================================================
    while (!glfwWindowShouldClose(window))
    {
        // -----------------------------
        // Time
        // -----------------------------
        float currentFrame =
            static_cast<float>(glfwGetTime());

        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // -----------------------------
        // Input
        // W X A D
        // -----------------------------
        processInput(window);

        // -----------------------------
        // Clear screen
        // -----------------------------
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

        // =====================================================
        // PYRAMID LIGHTING
        // =====================================================
        lightingShader.use();

        // LimeGreen
        lightingShader.setVec3(
            "objectColor",
            0.196078f,
            0.803922f,
            0.196078f
        );

        // LightSalmon
        lightingShader.setVec3("lightColor", 1.0f, 0.627451f, 0.478431f);
        lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setVec3("viewPos", camera.Position);

        // Projection
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // View
        glm::mat4 view = camera.GetViewMatrix(); lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
       
        // Model
        glm::mat4 model = glm::mat4(1.0f);lightingShader.setMat4("model", model);
        // Draw Pyramid
        glBindVertexArray(pyramidVAO);
        glDrawArrays(GL_TRIANGLES, 0, 18);
        // =====================================================
      // LIGHT OBJECT
      // =====================================================
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);

        model = glm::translate(
            model,
            lightPos
        );

        model = glm::scale(
            model,
            glm::vec3(0.2f)
        );

        lightCubeShader.setMat4(
            "model",
            model
        );

        glBindVertexArray(lightCubeVAO);

        glDrawArrays(
            GL_TRIANGLES,
            0,
            18
        );

        // -----------------------------
        // Swap buffers
        // -----------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // =========================================================
    // Cleanup
    // =========================================================
    glDeleteVertexArrays(1, &pyramidVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();

    return 0;
}

// =============================================================
// W X A D
// =============================================================
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


// =============================================================
// Window resize
// =============================================================
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
        
    

   
        
        
        
   

















        
           
        
          


      
        
          

            
           
   

        

        
           
         
     

       
       

      
       
       

      