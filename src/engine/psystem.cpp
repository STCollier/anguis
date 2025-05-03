#include "psystem.hpp"

#include <print>
#include <random>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

inline std::random_device rd;
inline std::mt19937 gen(rd());

Particle::Particle(glm::vec3 position, glm::vec3 color) : pos{position}, color{color} {
    glm::vec2 d1 = util::getRandomPointInCircle(32.0f);
    std::uniform_real_distribution<> d2(16, 32);
    std::uniform_real_distribution<> life(2.0, 4.0);
    
    acc = glm::vec3(0, -40.0f, 0);
    vel = glm::vec3(d1.x, d2(gen), d1.y);
    lifetime = life(gen);
    age = 0.0f;
}

void Particle::update(float dt) {
    vel += acc * dt;
    pos += vel * dt;

    if (pos.y <= 1.0f) {
        pos.y = 1.0f;
        vel.y = 0.0f;
        acc.y = 0.0f;
    }

    age += dt;
}

ParticleSystem::ParticleSystem(const std::string& modelSrc, size_t amount) : amount{amount}, done{false}, model{modelSrc, amount} {
    glm::vec3 rainbow[] = {
        {1.0f, 0.0f, 0.0f}, // Red
        {1.0f, 0.5f, 0.0f}, // Orange
        {1.0f, 1.0f, 0.0f}, // Yellow
        {0.0f, 1.0f, 0.0f}, // Green
        {0.0f, 0.0f, 1.0f}, // Blue
        {0.29f, 0.0f, 0.51f}, // Indigo
        {0.56f, 0.0f, 1.0f}  // Violet
    };

    for (size_t i = 0; i < amount; i++) {
        particles.push_back(Particle {glm::vec3(0.0, 10.0, 0.0), rainbow[rand() % (sizeof(rainbow) / sizeof(rainbow[0]))] * glm::vec3(10.0)});
    }

    remaining = std::vector<bool>(amount);
    std::fill(remaining.begin(), remaining.end(), false);
}

void ParticleSystem::reset() {
    particles.clear();

    glm::vec3 rainbow[] = {
        {1.0f, 0.0f, 0.0f}, // Red
        {1.0f, 0.5f, 0.0f}, // Orange
        {1.0f, 1.0f, 0.0f}, // Yellow
        {0.0f, 1.0f, 0.0f}, // Green
        {0.0f, 0.0f, 1.0f}, // Blue
        {0.29f, 0.0f, 0.51f}, // Indigo
        {0.56f, 0.0f, 1.0f}  // Violet
    };

    for (size_t i = 0; i < amount; i++) {
        particles.push_back(Particle {glm::vec3(0.0, 10.0, 0.0), rainbow[rand() % (sizeof(rainbow) / sizeof(rainbow[0]))] * glm::vec3(10.0)});
    }

    remaining = std::vector<bool>(amount);
    std::fill(remaining.begin(), remaining.end(), false);
}

void ParticleSystem::update(float dt) {
    matrices.clear();
    colors.clear();

    for (Particle& p : particles) {
        p.update(dt);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(p.pos));
        model = glm::scale(model, glm::vec3(0.1f));

        float alpha = glm::clamp(1.0f - (p.age / p.lifetime), 0.0f, 1.0f);

        matrices.push_back(model);
        colors.push_back(glm::vec4(p.color, alpha));
    }

    for (size_t i = 0; i < particles.size(); i++) if (particles[i].isDead()) remaining[i] = true;

    done = std::all_of(remaining.begin(), remaining.end(), [](bool b) { return b; });

    if (!done) model.updateInstanceData(amount, matrices, colors);
}

void ParticleSystem::render(Shader& shader) {
    if (!done) model.renderInstanced(shader, amount);
}

void ParticleSystem::use(Shader& shader, Camera& camera, Lighting& lighting, float dt) {
    shader.use();
    camera.setMatrices(shader);
    shader.setVec3("viewPos", camera.position);
    shader.setVec3("lightPos", lighting.lightPos);
    update(dt);
    render(shader);
}