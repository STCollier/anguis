#pragma once 

#include <glad/glad.h>

#include "model.hpp"
#include "util.hpp"
#include "camera.hpp"
#include "lighting.hpp"

class Particle {
    public:
        Particle(glm::vec3 position, glm::vec3 color);

        void update(float dt);
        
        bool isDead() { return age >= lifetime; }

        glm::vec3 acc, vel, pos;
        glm::vec3 color;
        float lifetime, age;

    private:
};

class ParticleSystem {
    public:
        ParticleSystem(const std::string& modelSrc, size_t amount);

        void reset();
        void update(float dt);
        void render(Shader& shader);
        void use(Shader& shader, Camera& camera, Lighting& lighting, float dt);

        size_t amount;
        std::vector<bool> remaining;
        bool done;

    private:
        Model model;

        std::vector<Particle> particles;
        std::vector<glm::mat4> matrices;
        std::vector<glm::vec4> colors;
};