#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "models.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "camera.hpp"

const glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightDir(-0.2f, -1.0f, -0.3f);

void drawParty(Camera& cam, unsigned int VAO, Shader& lightingShader, bool lightAtCam = false) {
    glBindVertexArray(VAO);
    lightingShader.use();
    lightingShader.setMatrix("view", cam.getViewMatrix());
    lightingShader.setMatrix("projection", glm::perspective(glm::radians(cam.getFov()), 800.0f / 600.0f, 0.1f, 100.0f));
    if (lightAtCam) {
        lightingShader.setBool("light.atCam", true);
    } else {
        lightingShader.setBool("light.atCam", false);
        lightingShader.setVec3("light.position", lightPos);
        lightingShader.setVec3("light.position", lightDir);
    }
    for (unsigned int i = 0; i < 10; i++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, (i % 3 == 0 ? (float)glfwGetTime() : 0.0f) * glm::radians(60.0f) + glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        lightingShader.setMatrix("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void drawLight(Camera& cam, unsigned int VAO, Shader& lightSrcShader) {
    glBindVertexArray(VAO);
    lightSrcShader.use();
    lightSrcShader.setMatrix("view", cam.getViewMatrix());
    lightSrcShader.setMatrix("projection", glm::perspective(glm::radians(cam.getFov()), 800.0f / 600.0f, 0.1f, 100.0f));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

Shader prepStaticLightSrc() {
    Shader lightSrcShader("../src/shaders/fullVtx.glsl", "../src/shaders/lightSrc.glsl");
    lightSrcShader.use();
    lightSrcShader.setVec3("lightColor", glm::vec3(1.0));
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    lightSrcShader.setMatrix("model", model);
    return lightSrcShader;
}

std::pair<Shader, std::vector<unsigned int>> prepParty(std::string lightType) {
    Shader lightingShader("../src/shaders/fullVtx.glsl", ("../src/shaders/lightTypes/" + lightType + ".glsl").c_str());

    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
    lightingShader.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", 32.0f);
    if (lightType == "direction") {
        lightingShader.setVec3("light.direction", lightDir);
    } else if (lightType == "point") {
        lightingShader.setVec3("light.position", lightPos);
        lightingShader.setFloat("light.constant",  1.0f);
        lightingShader.setFloat("light.linear",    0.045f);
        lightingShader.setFloat("light.quadratic", 0.0075f);	
    } else if (lightType == "spot") {
        lightingShader.setFloat("light.innerCone", glm::cos(glm::radians(12.5f)));
        lightingShader.setFloat("light.outerCone", glm::cos(glm::radians(17.5f)));
        lightingShader.setFloat("light.constant", 1.0f);
        lightingShader.setFloat("light.linear", 0.045f);
        lightingShader.setFloat("light.quadratic", 0.0075f);
    }
    lightingShader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    lightingShader.setVec3("light.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    lightingShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    // lightingShader.setMatrix("model", glm::mat4(1.0f));
    unsigned int diffuseMap = loadTexture("../public/container2.png");
    unsigned int specMap = loadTexture("../public/lighting_maps_specular_color.png");
    unsigned int emissionMap = loadTexture("../public/matrix.jpg");
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);
    lightingShader.setInt("material.emission", 2);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specMap);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, emissionMap);

    auto cube = createCubeWithNormTex();
    unsigned int cubeVAO = cube.second, cubeVtxCount = cube.first;
    std::vector<unsigned int> handles = { cubeVAO, cubeVtxCount };
    return std::make_pair(lightingShader, handles);
}