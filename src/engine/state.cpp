#include "state.hpp"

#include <GLFW/glfw3.h>

inline bool toggleESC = false;

void State::update(Window& window) {
    if (glfwGetKey(window.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS && !toggleESC) {
        mouseState = (MouseState) !mouseState;
        toggleESC = true;
    }
    if (glfwGetKey(window.getWindow(), GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
        toggleESC = false;
    }

    if (scene != MENU) glfwSetInputMode(window.getWindow(), GLFW_CURSOR, mouseState ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    else glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (mouseState == NORMAL && scene == GAME) scene = PAUSED;
    else if (mouseState == LOCKED && scene == PAUSED) scene = GAME;
}