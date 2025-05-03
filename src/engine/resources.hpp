#pragma once

#include "window.hpp"
#include "state.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "lighting.hpp"
#include "gui.hpp"
#include "client.hpp"
#include "psystem.hpp"

#include "../anguis/ground.hpp"
#include "../anguis/snake.hpp"
#include "../anguis/food.hpp"

class Resources {
    public:
        Resources();
        ~Resources();

        void beginUpdate();
        void render();
        void endUpdate();
        bool gameRunning() { return !glfwWindowShouldClose(window.getWindow()); }

    private:
        Window window;
        State state;

        struct Shaders {
            Shader main;
            Shader depth;
            Shader blur;
            Shader bloom;
            Shader glowline;
            Shader psystem;

        } shaders;

        struct GameObjects {
            Ground ground;
            Snake snake;
            Food food;
        } gameobjects;

        Camera camera;
        Lighting lighting;
        Gui gui;
        Client client;
        ParticleSystem psystem;

        std::vector<glm::mat4> pp;

        float dt;
        float time;

        void renderGameScene();
        void renderMenuScene();
};