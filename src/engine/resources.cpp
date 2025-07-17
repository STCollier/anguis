#include "resources.hpp"

Resources::Resources() : 
    window  {"Anguis", 1600, 900},
    state {},
    shaders {
        Shader("res/shaders/main.vert", "res/shaders/main.frag"),
        Shader("res/shaders/depth.vert", "res/shaders/depth.frag"),
        Shader("res/shaders/blur.vert", "res/shaders/blur.frag"),
        Shader("res/shaders/bloom.vert", "res/shaders/bloom.frag"),
        Shader("res/shaders/glowline.vert", "res/shaders/glowline.frag"),
        Shader("res/shaders/psystem.vert", "res/shaders/psystem.frag"),
        Shader("res/shaders/fade.vert", "res/shaders/fade.frag")
    },
    gameobjects {
        Ground{},
        Snake{},
        Food{}
    },
    camera {window},
    lighting {window, shaders.depth, shaders.blur, shaders.bloom},
    gui {window},
    client {}
{

    shaders.main.use();
    camera.setMatrices(shaders.main);
    shaders.main.setFloat("opacity", 1.0);
    shaders.main.setInt("_texture", 0);
    shaders.main.setInt("shadowMap", 1);
    shaders.main.setFloat("vignetteStrength", 0.75);
}

Resources::~Resources() {

}

void Resources::renderGameScene() {
    // First pass: render scene from perspective of light to depth buffer
    lighting.shadowPass(camera);

    glCullFace(GL_FRONT);
    gameobjects.ground.render(shaders.depth);
    gameobjects.snake.render(shaders.depth, shaders.fade, shaders.psystem, camera, lighting, state, dt);

    if (state.gameMode == MULTIPLAYER) client.renderPeers(shaders.depth, shaders.fade, shaders.psystem, camera, lighting, state, dt);

    gameobjects.food.render(shaders.depth, gameobjects.snake, time);
    glCullFace(GL_BACK);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_POLYGON_OFFSET_FILL);

    glm::ivec2 framebufferSize;
    glfwGetFramebufferSize(window.getWindow(), &framebufferSize.x, &framebufferSize.y);
    glViewport(0, 0, framebufferSize.x, framebufferSize.y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Second pass: render scene from normal camera's perspective to HDR framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, lighting.getHDRFBO());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    shaders.main.use();
    camera.setMatrices(shaders.main);
    shaders.main.setVec3("viewPos", camera.position);
    shaders.main.setVec3("lightPos", lighting.lightPos);
    shaders.main.setMat4("lightSpaceMatrix", lighting.lightSpaceMatrix);
    shaders.main.setVec2("resolution", glm::vec2(framebufferSize.x, framebufferSize.y));

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, lighting.getDepthBuffer());

    gameobjects.ground.render(shaders.main);
    gameobjects.snake.render(shaders.main, shaders.fade, shaders.psystem, camera, lighting, state, dt);

    if (state.gameMode == MULTIPLAYER) client.renderPeers(shaders.main, shaders.fade, shaders.psystem, camera, lighting, state, dt);
    
    gameobjects.food.render(shaders.main, gameobjects.snake, time);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Blur bright fragments using blur shader
    lighting.blurBloom();

    // Render out tonemapped HDR color
    lighting.renderColor();
}

void Resources::renderMenuScene() {
    glm::ivec2 framebufferSize;
    glfwGetFramebufferSize(window.getWindow(), &framebufferSize.x, &framebufferSize.y);

    shaders.glowline.use();
    shaders.glowline.setVec2("resolution", glm::vec2(framebufferSize.x, framebufferSize.y));
    shaders.glowline.setFloat("time", time);
    shaders.glowline.setVec3("col1", glm::vec3(0.66, 0.4, 1.0));
    shaders.glowline.setVec3("col2", glm::vec3(0.8, 0.2, 0.8));
    util::renderQuad();
}

void Resources::renderDeadScene() {
    state.mouseState = NORMAL;

    glm::ivec2 framebufferSize;
    glfwGetFramebufferSize(window.getWindow(), &framebufferSize.x, &framebufferSize.y);

    shaders.glowline.use();
    shaders.glowline.setVec2("resolution", glm::vec2(framebufferSize.x, framebufferSize.y));
    shaders.glowline.setFloat("time", time);

    shaders.glowline.setVec3("col1", glm::vec3(0.8, 0.1, 0.1));
    shaders.glowline.setVec3("col2", glm::vec3(1.0, 0.0, 0.1));

    util::renderQuad();
}

void Resources::render() {
    if (state.scene == GAME || state.scene == PAUSED) renderGameScene();
    else if (state.scene == MENU) renderMenuScene();
    else if (state.scene == DEAD) renderDeadScene();
}

void Resources::beginUpdate() {
    dt = window.deltaTime();
    time = window.time();

    glfwSwapInterval(state.settings.vsync);

    window.update();
    camera.update(window);
    state.update(window);

    if (state.gameMode == MULTIPLAYER) client.update(gameobjects.snake, dt);
}

void Resources::endUpdate() {
    if (glfwGetKey(window.getWindow(), GLFW_KEY_Q) == GLFW_PRESS || state.scene == QUIT) {
        glfwSetWindowShouldClose(window.getWindow(), 1);
    }

    shaders.bloom.setInt("useBloom", state.settings.bloom);
    if (state.scene == GAME) {
        gameobjects.snake.update(window, shaders.main, camera, dt);
    }

    //if (state.scene == MENU) gameobjects.snake.reset();


    gui.update(client, state, gameobjects.snake.score, dt);
    gui.render();

    glfwSwapBuffers(window.getWindow());
    glfwPollEvents();
}