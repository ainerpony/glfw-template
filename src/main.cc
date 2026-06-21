#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

#include <iostream>
#include <stdio.h>
#include <string>

int WIDTH  = 800*1.5;
int HEIGHT = 600*1.5;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
unsigned int genCanvas();

int main(int argc, char** argv)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Fail to create GLFW windows." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Load OpenGL function through GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Fail to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);

    // Setup Shader
    #if defined(_WIN32)
        std::string exeDir = std::string(argv[0]).substr(0, std::string(argv[0]).find_last_of("\\/")+1);
    #elif defined(__linux__)
        std::string exeDir = std::string(argv[0]).substr(0, std::string(argv[0]).find_last_of("/")+1);
    #endif

    Shader shader((exeDir+"shader.vert").c_str(), (exeDir+"shader.frag").c_str());
    shader.use();

    // Setup Canvas
    auto VAO = genCanvas();

    while(!glfwWindowShouldClose(window)) {
        processInput(window);
        if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
            shader.reloadShader((exeDir+"shader.vert").c_str(), (exeDir+"shader.frag").c_str());
            shader.use();
        }

        // Draw
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.setFloat("u_time", glfwGetTime());
        glUniform2f(glGetUniformLocation(shader.ID, "u_resolution"), WIDTH, HEIGHT); 

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    WIDTH  = width;
    HEIGHT = height;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

unsigned int genCanvas()
{
    float canvas[] = {
        -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f
    };
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(canvas), canvas, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Setup EBO
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Unbind VBO, VAO, EBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return VAO;
}