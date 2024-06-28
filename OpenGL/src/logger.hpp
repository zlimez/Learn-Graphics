#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <iomanip>
#include <glm/glm.hpp>
#include <unordered_set>

class Logger
{
public:
    static void printMat4(const glm::mat4 &matrix)
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                std::cout << std::setw(8) << std::setprecision(4) << matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    static void printVec3(const glm::vec3 &vector)
    {
        for (int i = 0; i < 3; ++i)
        {
            std::cout << std::setw(8) << std::setprecision(4) << vector[i] << " ";
        }
        std::cout << std::endl;
    }

    static void logOnce(const std::string &message)
    {
        static std::unordered_set<std::string> logged;
        if (logged.find(message) == logged.end())
        {
            std::cout << message << std::endl;
            logged.insert(message);
        }
    }
};

#endif