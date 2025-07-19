#pragma once

#include "../engine/model.hpp"
#include "../engine/texture.hpp"
#include "../engine/shader.hpp"
#include "../engine/audio.hpp"

#include "snake.hpp"

struct FoodPellet {
    glm::vec3 position;
    glm::vec3 color;
    float randomOffset;
};

class Food {
    public:
        Food();

        void render(Shader& shader, Snake& snake, Audio& audio, float time);
    
    private:
        Model m_model;
        Texture m_texture;
        std::vector<FoodPellet> foodPellets;

        void spawnPellet();
};