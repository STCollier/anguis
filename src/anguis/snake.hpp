#pragma once

#include "../engine/texture.hpp"
#include "../engine/shader.hpp"
#include "../engine/camera.hpp"
#include "../engine/window.hpp"
#include "../engine/model.hpp"
#include "../engine/lighting.hpp"
#include "../engine/psystem.hpp"
#include "../engine/state.hpp"
#include "../engine/audio.hpp"

#include <vector>

constexpr size_t START_SIZE = 10;

class Snake {
    public:
        Snake();
        Snake(std::vector<glm::vec2> positions);

        void setPositions(std::vector<glm::vec2> segmentPositions);
        void render(
            Shader& mainShader, 
            Shader& fadeShader, 
            Shader& particleShader,
            Camera& camera, 
            Lighting& lighting,
            State& state, 
            float dt
        );
        void update(Window& window, Shader& mainShader, Camera& camera, Audio& audio, float dt);
        bool collideWithOther(std::vector<glm::vec2>& pos);
        bool collide(glm::vec3 other, float radius);
        void grow();
        void die();
        void reset();

        std::vector<glm::vec2> positions;
        int score;
        const float snakeY = 2.0f;

    private:
        Model m_model;
        Texture m_texture_head;
        Texture m_texture_body;
        ParticleSystem psystem;
        

        void moveTo(float x, float y);
        void join();
        void slither(float x, float y);

        float speed;
        float currentSpeed, maxSpeed;
        float segmentRadius;
        bool dead;
        float afterDeathTimer = 0.0f;

        [[maybe_unused]] float dx, dy;
        glm::vec2 position; // ultimate position of snake (head)
};