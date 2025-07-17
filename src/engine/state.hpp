#pragma once

#include "window.hpp"

enum MouseState {
    LOCKED,
    NORMAL
};

enum GameMode {
    SINGLEPLAYER,
    MULTIPLAYER
};

enum Scene {
    GAME,
    PAUSED,
    MENU,
    DEAD,
    SELECT,
    QUIT
};

struct Settings {
    bool vsync = true;
    bool bloom = true;
    bool FPS = true;
};

class State {
    public:
        State() = default;
        
        void update(Window& window);

        Scene scene = MENU;
        MouseState mouseState = LOCKED;
        GameMode gameMode = SINGLEPLAYER;
        Settings settings;
};