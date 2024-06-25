#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "models.hpp"
#include "definitions/shader.hpp"
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

const glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f, 0.2f, 2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f, 2.0f, -12.0f),
    glm::vec3(0.0f, 0.0f, -3.0f)
};

glm::vec3 pointLightColors[] = {
    glm::vec3(0.1f, 0.1f, 0.1f),
    glm::vec3(0.1f, 0.1f, 0.1f),
    glm::vec3(0.1f, 0.1f, 0.1f),
    glm::vec3(0.3f, 0.1f, 0.1f)
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
        lightingShader.setVec3("light.direction", lightDir);
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

void drawPartyCL(Camera& cam, unsigned int VAO, Shader& lightingShader) {
    glBindVertexArray(VAO);
    lightingShader.use();
    lightingShader.setMatrix("view", cam.getViewMatrix());
    lightingShader.setMatrix("projection", glm::perspective(glm::radians(cam.getFov()), 800.0f / 600.0f, 0.1f, 100.0f));
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

void drawPtLights(Camera& cam, unsigned int VAO, Shader& lightSrcShader) {
    glBindVertexArray(VAO);
    lightSrcShader.use();
    lightSrcShader.setMatrix("view", cam.getViewMatrix());
    lightSrcShader.setMatrix("projection", glm::perspective(glm::radians(cam.getFov()), 800.0f / 600.0f, 0.1f, 100.0f));
    for (int i = 0; i < 4; i++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[i]);
        model = glm::scale(model, glm::vec3(0.2f));
        lightSrcShader.setVec3("lightColor", pointLightColors[i]);
        lightSrcShader.setMatrix("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
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

Shader prepHorrorLight() {
    Shader lightingShader("../src/shaders/fullVtx.glsl", "../src/shaders/lightTypes/combined.glsl");
    lightingShader.use();
    // Directional light
    lightingShader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));		
    lightingShader.setVec3("dirLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));	
    lightingShader.setVec3("dirLight.diffuse", glm::vec3(0.05f, 0.05f, 0.05)); 
    lightingShader.setVec3("dirLight.specular", glm::vec3(0.2f, 0.2f, 0.2f));
    // Point light 1
    lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);		
    lightingShader.setVec3("pointLights[0].ambient", glm::vec3(0.1) * pointLightColors[0]);		
    lightingShader.setVec3("pointLights[0].diffuse", pointLightColors[0]); 
    lightingShader.setVec3("pointLights[0].specular", pointLightColors[0]);
    lightingShader.setFloat("pointLights[0].constant", 1.0f);
    lightingShader.setFloat("pointLights[0].linear", 0.14);
    lightingShader.setFloat("pointLights[0].quadratic", 0.07);		
    // Point light 2
    lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);		
    lightingShader.setVec3("pointLights[1].ambient", glm::vec3(0.1) * pointLightColors[1]);		
    lightingShader.setVec3("pointLights[1].diffuse", pointLightColors[1]); 
    lightingShader.setVec3("pointLights[1].specular", pointLightColors[1]);
    lightingShader.setFloat("pointLights[1].constant", 1.0f);
    lightingShader.setFloat("pointLights[1].linear", 0.14);
    lightingShader.setFloat("pointLights[1].quadratic", 0.07);		
    // Point light 3
    lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);		
    lightingShader.setVec3("pointLights[2].ambient",  glm::vec3(0.1) * pointLightColors[2]);		
    lightingShader.setVec3("pointLights[2].diffuse", pointLightColors[2]); 
    lightingShader.setVec3("pointLights[2].specular" ,pointLightColors[2]);
    lightingShader.setFloat("pointLights[2].constant", 1.0f);
    lightingShader.setFloat("pointLights[2].linear", 0.22);
    lightingShader.setFloat("pointLights[2].quadratic", 0.20);		
    // Point light 4
    lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);		
    lightingShader.setVec3("pointLights[3].ambient",  glm::vec3(0.1) * pointLightColors[3]);		
    lightingShader.setVec3("pointLights[3].diffuse", pointLightColors[3]); 
    lightingShader.setVec3("pointLights[3].specular", pointLightColors[3]);
    lightingShader.setFloat("pointLights[3].constant", 1.0f);
    lightingShader.setFloat("pointLights[3].linear", 0.14);
    lightingShader.setFloat("pointLights[3].quadratic", 0.07);		
    // flashLight
    lightingShader.setVec3("flashLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));	
    lightingShader.setVec3("flashLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f)); 
    lightingShader.setVec3("flashLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    lightingShader.setFloat("flashLight.constant", 1.0f);
    lightingShader.setFloat("flashLight.linear", 0.09);
    lightingShader.setFloat("flashLight.quadratic", 0.032);			
    lightingShader.setFloat("flashLight.innerCone", glm::cos(glm::radians(10.0f)));
    lightingShader.setFloat("flashLight.outerCone", glm::cos(glm::radians(15.0f)));
    return lightingShader;
}
