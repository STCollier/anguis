#pragma once

#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <math.h>
#include <glm/glm.hpp>
#include <random>

namespace util {
    constexpr float TAU = M_PI * 2;

    std::string slurp(std::ifstream& in);
    size_t strSpaces(std::string str);
    glm::vec2 getRandomPointInCircle(float radius);
    void renderQuad();
    float lerp(float v0, float v1, float t);
    float easeInOutSine(float x);
};