#ifndef UTILS_H
#define UTILS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void setMatrixCol(glm::mat4 &matrix, glm::vec3 row, int index) {
    matrix[index][0] = row.x;
    matrix[index][1] = row.y;
    matrix[index][2] = row.z;
}

void setMatrixRow(glm::mat4 &matrix, glm::vec3 col, int index) {
    matrix[0][index] = col.x;
    matrix[1][index] = col.y;
    matrix[2][index] = col.z;
}

#endif