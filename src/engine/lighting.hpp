#pragma once

#include "shader.hpp"
#include "camera.hpp"
#include "window.hpp"
#include "util.hpp"

class Lighting {
    public:
        Lighting(Window& window, Shader& _depthShader, Shader& _blurShader, Shader& _bloomShader);

        void shadowPass(Camera& camera);
        void blurBloom();
        void renderColor();

        unsigned int getDepthFBO() { return depthFBO; }
        unsigned int getDepthBuffer()    { return depthBuffer; }

        unsigned int getHDRFBO() { return HDRFBO; }
        unsigned int getColorBuffer() { return colorBuffer; }

        float shadowResolution;
        float exposure;
        int blurIterations;
        glm::mat4 lightSpaceMatrix;
        glm::vec3 lightPos;

    private:
        unsigned int depthFBO, depthBuffer;
        unsigned int HDRFBO, renderBuffer, colorBuffer;
        unsigned int colorBuffers[2];
        unsigned int pingpongFBO[2];
        unsigned int pingpongColorbuffers[2];

        bool horizontal, firstIteration;

        Shader depthShader, blurShader, bloomShader;
};