#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <iomanip>
#include <glm/glm.hpp>

void printMat4(const glm::mat4& matrix) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << std::setw(8) << std::setprecision(4) << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

#endif