#pragma once

#include "../engine/texture.hpp"
#include "../engine/shader.hpp"
#include "../engine/camera.hpp"
#include "../engine/window.hpp"
#include "../engine/model.hpp"
#include "../engine/lighting.hpp"

#include <vector>

class Snake {
    public:
        Snake();
        Snake(std::vector<glm::vec2> positions);

        void setPositions(std::vector<glm::vec2> segmentPositions);
        void render(Shader& shader);
        void update(Window& window, Shader& mainShader, Camera& camera, float dt);
        bool collide(glm::vec3 other, float radius);
        void grow();
        void die();
        void reset();

        std::vector<glm::vec2> positions;
        const float snakeY = 2.0f;

    private:
        Model m_model;
        Texture m_texture_head;
        Texture m_texture_body;

        void moveTo(float x, float y);
        void join();
        void slither(float x, float y);

        float speed;
        float currentSpeed, maxSpeed;
        float segmentRadius;
        bool dead;

        [[maybe_unused]] float dx, dy;
        glm::vec2 position; // ultimate position of snake (head)
};