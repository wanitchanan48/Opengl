// lab04ColorPolygon.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;


// Vertex Shader
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"flat out vec3 ourColor;\n"

"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";


// Fragment Shader
const char* fragmentShaderSource = "#version 330 core\n"
"flat in vec3 ourColor;\n"
"out vec4 FragColor;\n"

"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0);\n"
"}\0";
int main()
{
    // Initialize GLFW
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(
        SCR_WIDTH,
        SCR_HEIGHT,
        "Gradient Polygon",
        NULL,
        NULL
    );


    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }


    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // Load GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }



    // -------------------------
    // Create Shader
    // -------------------------

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(
        vertexShader,
        1,
        &vertexShaderSource,
        NULL
    );

    glCompileShader(vertexShader);



    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(
        fragmentShader,
        1,
        &fragmentShaderSource,
        NULL
    );
    glCompileShader(fragmentShader);
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // -------------------------
    // Vertex Data
    // ไม่ใช้ indices
    // -------------------------

    float vertices[] =
    {
        // Position               // Color
         0.000f,  0.700f, 0.0f,    0.9f, 0.0f, 0.0f,   // แดง
         0.411f,  0.566f, 0.0f,    1.0f, 0.5f, 0.0f,   // ส้ม
         0.665f,  0.216f, 0.0f,    1.0f, 1.0f, 0.0f,   // เหลือง
         0.665f, -0.216f, 0.0f,    0.0f, 1.0f, 0.0f,   // เขียว
         0.411f, -0.566f, 0.0f,    0.0f, 1.0f, 1.0f,   // ฟ้า
         0.000f, -0.700f, 0.0f,    0.0f, 0.4f, 1.0f,   // น้ำเงิน
        -0.411f, -0.566f, 0.0f,    0.4f, 0.0f, 1.0f,   // ม่วง
        -0.665f, -0.216f, 0.0f,    0.7f, 0.0f, 1.0f,   // ม่วงเข้ม
        -0.665f,  0.216f, 0.0f,    1.0f, 0.0f, 0.6f,   // ชมพูเข้ม
        -0.411f,  0.566f, 0.0f,    1.0f, 0.2f, 0.5f    // ชมพู
    };


    unsigned int VAO;
    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
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
    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.92f, 0.80f, 0.95f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // ไม่ใช้ glDrawElements
        // ใช้ Triangle Fan แทน
        glDrawArrays(
            GL_TRIANGLE_FAN,
            0,
            10
        );
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // Delete resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return 0;
}
// Keyboard input
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
// Resize window
void framebuffer_size_callback(
    GLFWwindow* window,
    int width,
    int height
)
{
    glViewport(0, 0, width, height);
}