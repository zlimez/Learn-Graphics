#ifndef LIGHTSRC_H
#define LIGHTSRC_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class LightSrc {
public:
    glm::vec3 position;
    glm::vec3 color;
    LightSrc(glm::vec3 position, glm::vec3 color);
};

#endif