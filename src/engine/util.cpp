#include "util.hpp"

namespace util {
    std::string slurp(std::ifstream& in) {
        std::ostringstream sstr;
        sstr << in.rdbuf();
        return sstr.str();
    }

    size_t strSpaces(std::string str) {
        size_t x = 0;
        for (char& c : str) {
            if (c == 32) x++;
        }

        return x;
    }

    glm::vec2 getRandomPointInCircle(float radius) {
        glm::vec2 center = glm::vec2(0.0, 0.0);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);

        float r = radius * sqrt(dist(gen));
        float theta = dist(gen) * util::TAU;

        float x = center.x + r * cosf(theta);
        float y = center.y + r * sinf(theta);

        return {x, y};
    }

    float lerp(float v0, float v1, float t) {
        return (1 - t) * v0 + t * v1;
    }

    float easeInOutSine(float x) {
        return -(cosf(M_PI * x) - 1) / 2;
    }

    void renderQuad() {
        unsigned int quadVAO = 0, quadVBO;
        
        if (!quadVAO) {
            float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            };

            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        }

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }
};