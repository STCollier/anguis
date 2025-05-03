#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../engine/util.hpp"
#include "ground.hpp"


void Ground::render(Shader& shader) {
    glActiveTexture(GL_TEXTURE0);
    m_texture.bind();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.f, 0.f, 0.f));
    model = glm::scale(model, glm::vec3(16.f));

    m_model.render(shader, model);
}