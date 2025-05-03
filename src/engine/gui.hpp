#pragma once

#include <functional>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_stdlib.h"

#include "window.hpp"
#include "state.hpp"
#include "texture.hpp"
#include "client.hpp"

class Gui {
    public:
        Gui(Window& window);
        ~Gui();

        void update(Client& client, State& state, float dt);
        void render();

    private:
        ImFont* font_title;
        ImFont* font_small;
        ImFont* font_default;
        ImFont* font_title_large;
        Texture titleTexture;

        void pauseMenu(ImGuiIO& io, State& state, Client& client);
        void startMenu(ImGuiIO& io, State& state);
        void selectMenu(ImGuiIO& io, State& state, Client& client);
        void chatbox(ImGuiIO& io, State& state, Client& client);
};