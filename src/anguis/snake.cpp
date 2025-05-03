#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui/imgui.h"

#include "../engine/util.hpp"
#include "snake.hpp"

Snake::Snake() 
: m_model("res/models/snake.obj"), m_texture_head("res/textures/snake.hdr", NEAREST), m_texture_body("res/textures/snake_purple.hdr", NEAREST), dx{0.0f}, dy{0.0f}, position{0.f, 0.f} {
    positions.push_back(position);

    segmentRadius = 1.0f;
    currentSpeed = 32.0f;
    maxSpeed = 45.0f;
    dead = false;

    speed = currentSpeed;
    
    for (int i = 1; i < 10; i++) {
        positions.push_back(glm::vec2(position.x, position.y + static_cast<float>(i) * segmentRadius * 2));
    }
}

Snake::Snake(std::vector<glm::vec2> segmentPositions) 
: m_model("res/models/snake.obj"), m_texture_head("res/textures/snake.hdr", NEAREST), m_texture_body("res/textures/snake_purple.hdr", NEAREST), dx{0.0f}, dy{0.0f} {
    position = segmentPositions[0];

    segmentRadius = 1.0f;
    currentSpeed = 32.0f;
    maxSpeed = 45.0f;
    dead = false;

    speed = currentSpeed;

    for (glm::vec2& pos : segmentPositions) {
        positions.push_back(pos);
    }
}

void Snake::setPositions(std::vector<glm::vec2> segmentPositions) {
    position = segmentPositions[0];

    positions.clear();
    for (glm::vec2& pos : segmentPositions) {
        positions.push_back(pos);
    }
}

static bool firstPerson = false;
static bool toggled = false;
static float l = 0;
static float vig = 0.7;

void Snake::update(Window& window, Shader& mainShader, Camera& camera, float dt) {
    // Third person movement with WASD
    /*if (window.keyDown(GLFW_KEY_W)) dy -= 1.0f;
    if (window.keyDown(GLFW_KEY_S)) dy += 1.0f;
    if (window.keyDown(GLFW_KEY_A)) dx -= 1.0f;
    if (window.keyDown(GLFW_KEY_D)) dx += 1.0f;

    float length = std::sqrt(dx * dx + dy * dy);
    if (length != 0.0f) {
        dx /= length;
        dy /= length;
    }

    position.x += dx * speed * dt;
    position.y += dy * speed * dt;*/

    bool interactingWithUI = ImGui::IsAnyItemActive() || ImGui::IsAnyItemHovered();

    if (window.mouseDown(GLFW_MOUSE_BUTTON_LEFT) && !interactingWithUI) {
        l < 1.0 ? l += dt * 2.0 : l = 1.0;
        speed = util::lerp(currentSpeed, maxSpeed, util::easeInOutSine(l));
        camera.FOV = util::lerp(45.0f, 60.0f, util::easeInOutSine(l));
        vig = util::lerp(0.75, 0.6, util::easeInOutSine(l));
    } else {
        l > 0.0 ? l -= dt * 2.0 : l = 0.0;
        speed = util::lerp(currentSpeed, maxSpeed, util::easeInOutSine(l));
        camera.FOV = util::lerp(45.0f, 60.0f, util::easeInOutSine(l));
        vig = util::lerp(0.75, 0.6, util::easeInOutSine(l));
    }

    mainShader.use();
    mainShader.setFloat("vignetteStrength", vig);

    if (window.keyDown(GLFW_KEY_TAB) && !toggled) {
        firstPerson = !firstPerson;
        toggled = true;
    }

    if (!window.keyDown(GLFW_KEY_TAB) && toggled) toggled = false;

    glm::vec2 direction = glm::normalize(glm::vec2(camera.forward.x, camera.forward.z));
    /*if (!dead)*/ position += direction * speed * dt;

    camera.view = glm::mat4(1.0f);

    if (firstPerson) {
        glm::vec3 camPos = glm::vec3(position.x, snakeY, position.y);
        glm::vec3 direction3D = glm::normalize(glm::vec3(direction.x, 0.0f, direction.y));
        glm::vec3 camTarget = camPos + direction3D;
        
        camera.view = glm::lookAt(camPos, camTarget, glm::vec3(0.0f, 1.0f, 0.0f));
        camera.position = camPos;
    } else {
        camera.view = glm::rotate(camera.view, (float) glm::radians(45.0), glm::vec3(1.f, 0.f, 0.f));
        camera.view = glm::translate(camera.view, glm::vec3(0.0f, -100.0f, -100.0f));
    }

    for (size_t i = 1; i < positions.size(); i++) {
        if (collide(glm::vec3(positions[i].x, snakeY, positions[i].y), segmentRadius * 0.95)) {
            die();
        }
    }

    if (!collide(glm::vec3(0.0, 2.0, 0.0), 115.0f)) {
        die();
    }

    slither(position.x, position.y);

    /*if (window.keyDown(GLFW_KEY_SPACE)) {
        grow();
    }*/
}

void Snake::render(Shader& shader) {
    glActiveTexture(GL_TEXTURE0);

    for (size_t i = 0; i < positions.size(); i++) {
        !i ? m_texture_head.bind() : m_texture_body.bind();
    
        float angle = 0.0f;
        if (i < positions.size() - 1) {
            glm::vec2 nextSegment = positions[i + 1];
            glm::vec2 thisSegment = positions[i];
    
            float dx = nextSegment.x - thisSegment.x;
            float dy = nextSegment.y - thisSegment.y;
    
            angle = atan2f(dy, dx);
        } else if (i > 0) {
            glm::vec2 thisSegment = positions[i];
            glm::vec2 prevSegment = positions[i - 1];
    
            float dx = thisSegment.x - prevSegment.x;
            float dy = thisSegment.y - prevSegment.y;
    
            angle = atan2f(dy, dx);
        }
    
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(positions[i].x, snakeY, positions[i].y));
        model = glm::rotate(model, angle, glm::vec3(0.f, 1.f, 0.f));
        model = glm::scale(model, glm::vec3(segmentRadius * 2));
    
        m_model.render(shader, model);
    }
}

void Snake::moveTo(float x, float y) {
    positions[0].x = x;
    positions[0].y = y;

    join();
}

void Snake::join() {
    for (size_t i = 1; i < positions.size(); i++) {
        glm::vec2 last = positions[i - 1];
        glm::vec2& curr = positions[i];
    
        // get difference in x and y of each position
        float _dx = curr.x - last.x;
        float _dy = curr.y - last.y;
    
        // calculate the angle between the two parts of the snake
        float angle = atan2f(_dy, _dx);
    
        // get the new x and new y using polar coordinates
        float nx = 2 * segmentRadius * cosf(angle);
        float ny = 2 * segmentRadius * sinf(angle);
    
        // add the new x and new y to the last snake's position to "join" the two together without a gap
        curr.x = nx + last.x;
        curr.y = ny + last.y;
    }
}

void Snake::slither(float x, float y) {
    glm::vec2 head = positions[0];
    
    // smooth movement
    float nx = head.x + (x - head.x) / 10;
    float ny = head.y + (y - head.y) / 10;
    
    moveTo(nx, ny);
}

void Snake::grow() {
    glm::vec2 tail = positions.back();

    glm::vec2 penultimateTail = positions[positions.size() - 2];
    float dx = tail.x - penultimateTail.x;
    float dy = tail.y - penultimateTail.y;


    glm::vec2 newSegment = tail + glm::vec2(dx, dy);
    positions.push_back(newSegment);
}

void Snake::die() {
    dead = true;
}

// Based on head position
bool Snake::collide(glm::vec3 other, float radius) {
    glm::vec2 head = positions[0];

    glm::vec3 dist = glm::vec3(head.x - other.x, snakeY - other.y, head.y - other.z);
    float distance = sqrt((dist.x * dist.x) + (dist.y * dist.y) + (dist.z * dist.z));
  
    return (distance <= segmentRadius + radius);
}

void Snake::reset() {
    dx = 0.0f;
    dy = 0.0f;
    position = {0.0f, 0.0f};

    positions.clear();
    
    for (int i = 1; i < 16; i++) {
        positions.push_back(glm::vec2(0.0f, static_cast<float>(i) * segmentRadius * 2));
    }
}