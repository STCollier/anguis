#pragma once

#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <math.h>
#include <glm/glm.hpp>
#include <random>

namespace util {
    constexpr float TAU = M_PI * 2;
    constexpr glm::vec2 GLM_VEC2_ZERO = glm::vec2(0.0f);
    constexpr glm::vec3 GLM_VEC3_ZERO = glm::vec3(0.0f);

    std::string slurp(std::ifstream& in);
    size_t strSpaces(std::string str);
    glm::vec2 getRandomPointInCircle(glm::vec2 center, float radius);
    void renderQuad();
    float lerp(float v0, float v1, float t);
    float easeInOutSine(float x);
};