#pragma once

#include <glm/glm.hpp>

#include "shader.hpp"

#include <string>
#include <vector>
#include <filesystem>

struct Vertex {
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;
};

struct InstanceData {
    glm::mat4 modelMatrix;
    glm::vec4 color;
};

class Model {
    public:
        Model(const std::string& src);
        // Used for loading instanced data
        Model(const std::string& src, size_t instanceCount);
        ~Model();

        void updateInstanceData(size_t count, std::vector<glm::mat4>& matrices, std::vector<glm::vec4>& colors);
        void renderInstanced(Shader& shader, size_t count);

        void render(Shader& shader, glm::mat4 model);


    private:
        void loadData(const std::string& src);
        void bind();

        std::vector<struct Vertex> vertices;

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;

        std::vector<unsigned int> positionIndices, uvIndices, normalIndices;

        unsigned int VBO, VAO;
        [[maybe_unused]] unsigned int instanceVBO;
        size_t vertexCount;
};