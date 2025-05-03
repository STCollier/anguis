#pragma once

#include "../engine/texture.hpp"
#include "../engine/shader.hpp"
#include "../engine/model.hpp"

class Ground {
    public:
        Ground() : m_model("res/models/arena.obj"), m_texture("res/textures/albedo.hdr", LINEAR) {};

        void render(Shader& shader);

    private:
        Model m_model;
        Texture m_texture;
};