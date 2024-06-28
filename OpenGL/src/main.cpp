#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.hpp"
#include "definitions/model.hpp"
#include "samples.hpp"

struct MouseInput
{
    Camera *cam;
    bool firstMouse = true;
    float lastX = 400.0f;
    float lastY = 300.0f;
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); }

void error_callback(int error, const char *description) { std::cerr << "GLFW Error: " << description << std::endl; }

void processInput(GLFWwindow *window, float &visibilityRatio, Camera &cam, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        visibilityRatio += 0.05f;
        if (visibilityRatio > 1.0f)
        {
            visibilityRatio = 1.0f;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        visibilityRatio -= 0.05f;
        if (visibilityRatio < 0.0f)
        {
            visibilityRatio = 0.0f;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cam.processKeyboard(Camera::FORWARD, deltaTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cam.processKeyboard(Camera::BACKWARD, deltaTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cam.processKeyboard(Camera::LEFT, deltaTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cam.processKeyboard(Camera::RIGHT, deltaTime);
    }
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
    MouseInput *input = static_cast<MouseInput *>(glfwGetWindowUserPointer(window));
    if (input && input->cam)
    {
        if (input->firstMouse)
        {
            input->lastX = static_cast<float>(xpos);
            input->lastY = static_cast<float>(ypos);
            input->firstMouse = false;
        }

        float xoffset = static_cast<float>(xpos) - input->lastX;
        float yoffset = input->lastY - static_cast<float>(ypos);
        input->lastX = static_cast<float>(xpos);
        input->lastY = static_cast<float>(ypos);

        input->cam->processMouseMovement(xoffset, yoffset);
    }
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    MouseInput *input = static_cast<MouseInput *>(glfwGetWindowUserPointer(window));
    if (input && input->cam)
        input->cam->processMouseScroll(yoffset);
}

int main()
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glClearColor(0.5f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    float visibilityRatio = 0.5f;

    Camera cam(glm::vec3(0.0f, 0.0f, 3.0f), -90.0f, 0.0f, false);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    MouseInput mouseInput = {&cam};
    glfwSetWindowUserPointer(window, &mouseInput);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // auto handles = prepPartyCL();

    // relative path from bin directory
    Shader shader("../src/shaders/modelDemo/vtx.glsl", "../src/shaders/modelDemo/frag.glsl");
    char path[] = "../public/backpack/backpack.obj";
    Model backpack(path);
    auto model = glm::mat4(1.0f);
    shader.use();
    model = glm::translate(model, glm::vec3(0.0f));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMatrix("model", model);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, visibilityRatio, cam, deltaTime);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.setMatrix("view", cam.getViewMatrix());
        shader.setMatrix("projection", glm::perspective(glm::radians(cam.getFov()), 800.0f / 600.0f, 0.1f, 100.0f));
        backpack.Draw(shader);
        // handles.first.use();
        // drawPartyCL(cam, handles.second[0], handles.first);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}