#pragma once

#include "../engine/shader.hpp"
#include "window.hpp"

class Camera {
    public:
        Camera(Window& window);

        void setMatrices(Shader& shader);
        void update(Window& window);
        
        glm::mat4 projection;
        glm::mat4 view;
        glm::vec3 forward;
        glm::vec3 position;

        float near, far, FOV, sensitivity;

    private:
        float lastX, lastY, pitch, yaw;
};