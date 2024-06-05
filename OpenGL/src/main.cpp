#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.hpp"
#include "models.hpp"
#include "texture.hpp"
#include "lightSrc.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.hpp"
#include "logger.hpp"

struct MouseInput {
    Camera* cam;
    bool firstMouse = true;
    float lastX = 400.0f;
    float lastY = 300.0f;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }

void error_callback(int error, const char* description) { std::cerr << "GLFW Error: " << description << std::endl; }

void processInput(GLFWwindow *window, float &visibilityRatio, Camera &cam, float deltaTime)
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
    } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cam.processKeyboard(Camera::FORWARD, deltaTime);
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cam.processKeyboard(Camera::BACKWARD, deltaTime);
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cam.processKeyboard(Camera::LEFT, deltaTime);
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cam.processKeyboard(Camera::RIGHT, deltaTime);
    }
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    MouseInput* input = static_cast<MouseInput*>(glfwGetWindowUserPointer(window));
    if (input && input->cam) {
        if (input->firstMouse) {
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

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    MouseInput* input = static_cast<MouseInput*>(glfwGetWindowUserPointer(window));
    if (input && input->cam) input->cam->processMouseScroll(yoffset);
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
    glEnable(GL_DEPTH_TEST);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // Shader shader("../src/shaders/fullVtx.glsl", "../src/shaders/textureEx.glsl");
    // tutTexture(shader);
    Shader lightingShader("../src/shaders/fullVtx.glsl", "../src/shaders/lightObj.glsl"), lightSrcShader("../src/shaders/fullVtx.glsl", "../src/shaders/lightSrc.glsl");
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    LightSrc lightSrc(lightPos, lightColor);

    lightingShader.use();
    lightingShader.setVec3("lightColor", lightColor);
    lightingShader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
    lightSrcShader.use();
    lightSrcShader.setVec3("lightColor", lightColor);

    auto cube = createCube(), lightCube = createCube();
    int cubeVAO = cube.second, cubeVtxCount = cube.first;
    int lightCubeVAO = lightCube.second, lightCubeVtxCount = lightCube.first;
    float visibilityRatio = 0.5f;
    // glm::vec3 cubePositions[] = {
    //     glm::vec3( 0.0f,  0.0f,  0.0f), 
    //     glm::vec3( 2.0f,  5.0f, -15.0f), 
    //     glm::vec3(-1.5f, -2.2f, -2.5f),  
    //     glm::vec3(-3.8f, -2.0f, -12.3f),  
    //     glm::vec3( 2.4f, -0.4f, -3.5f),  
    //     glm::vec3(-1.7f,  3.0f, -7.5f),  
    //     glm::vec3( 1.3f, -2.0f, -2.5f),  
    //     glm::vec3( 1.5f,  2.0f, -2.5f), 
    //     glm::vec3( 1.5f,  0.2f, -1.5f), 
    //     glm::vec3(-1.3f,  1.0f, -1.5f)  
    // };

    Camera cam(glm::vec3(0.0f, 0.0f, 3.0f), -90.0f, 0.0f, false);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    MouseInput mouseInput = { &cam };
    glfwSetWindowUserPointer(window, &mouseInput);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window, visibilityRatio, cam, deltaTime);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        // shader.setFloat("visibilityRatio", visibilityRatio);

        // glm::mat4 trans = glm::mat4(1.0f);
        // trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
        // trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        // shader.setMatrix("transform", trans);
        // glDrawElements(GL_TRIANGLES, vtxCount, GL_UNSIGNED_INT, 0);

        // glm::mat4 trans2 = glm::mat4(1.0f);
        // trans2 = glm::translate(trans2, glm::vec3(-0.5, 0.5, 0.0));
        // trans2 = glm::scale(trans2, sinf((float)glfwGetTime()) * glm::vec3(0.5, 0.5, 0.5) + glm::vec3(1.0, 1.0, 1.0));
        // shader.setMatrix("transform", trans2);
        // glDrawElements(GL_TRIANGLES, vtxCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(cubeVAO);
        lightingShader.use();
        lightingShader.setMatrix("view", cam.getViewMatrix());
        lightingShader.setMatrix("projection", glm::perspective(glm::radians(cam.getFov()), 800.0f / 600.0f, 0.1f, 100.0f));
        lightingShader.setMatrix("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, cubeVtxCount);

        glBindVertexArray(lightCubeVAO);
        lightSrcShader.use();
        lightSrcShader.setMatrix("view", cam.getViewMatrix());
        lightSrcShader.setMatrix("projection", glm::perspective(glm::radians(cam.getFov()), 800.0f / 600.0f, 0.1f, 100.0f));
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightSrcShader.setMatrix("model", model);
        glDrawArrays(GL_TRIANGLES, 0, lightCubeVtxCount);

        // for (unsigned int i = 0; i < 10; i++) {
        //     glm::mat4 model = glm::mat4(1.0f);
        //     model = glm::translate(model, cubePositions[i]);
        //     float angle = 20.0f * i;
        //     model = glm::rotate(model, (i % 3 == 0 ? (float)glfwGetTime() : 0.0f) * glm::radians(60.0f) + glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        //     shader.setMatrix("model", model);
        //     glDrawArrays(GL_TRIANGLES, 0, vtxCount);
        // }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}