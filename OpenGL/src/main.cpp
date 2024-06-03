#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.hpp"
#include "triangles.hpp"
#include "texture.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void error_callback(int error, const char* description) {
    std::cerr << "GLFW Error: " << description << std::endl;
}

void processInput(GLFWwindow *window, float &visibilityRatio)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    } else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        visibilityRatio += 0.05f;
        if (visibilityRatio > 1.0f) {
            visibilityRatio = 1.0f;
        }
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        visibilityRatio -= 0.05f;
        if (visibilityRatio < 0.0f) {
            visibilityRatio = 0.0f;
        }
    }
}

int main() {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Shader shader("../src/shaders/vtxToColor.glsl", "../src/shaders/colorAsVtx.glsl");
    // Shader shader("../src/shaders/xOffset.glsl", "../src/shaders/colorAsVtx.glsl");
    // shader.use();
    // shader.setFloat("xOffset", 0.5f);
    Shader shader("../src/shaders/textureVtx.glsl", "../src/shaders/textureEx.glsl");
    tutTexture(shader);
    int vtxCount = createRect();
    float visibilityRatio = 0.5f;

    while (!glfwWindowShouldClose(window)) {
        processInput(window, visibilityRatio);
        glClear(GL_COLOR_BUFFER_BIT);
        shader.setFloat("visibilityRatio", visibilityRatio);

        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        shader.setMatrix("transform", trans);
        glDrawElements(GL_TRIANGLES, vtxCount, GL_UNSIGNED_INT, 0);

        glm::mat4 trans2 = glm::mat4(1.0f);
        trans2 = glm::translate(trans2, glm::vec3(-0.5, 0.5, 0.0));
        trans2 = glm::scale(trans2, sinf((float)glfwGetTime()) * glm::vec3(0.5, 0.5, 0.5) + glm::vec3(1.0, 1.0, 1.0));
        shader.setMatrix("transform", trans2);
        glDrawElements(GL_TRIANGLES, vtxCount, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}