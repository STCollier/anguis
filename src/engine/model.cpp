#include <glad/glad.h>

#include "model.hpp"
#include "util.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

void Model::loadData(const std::string& src) {
    std::ifstream modelFile(src);

    if (!modelFile) throw std::runtime_error(std::format("Failed to load model file {}\n", src));

    std::filesystem::path path = src;
    if (path.extension() != ".obj") throw std::runtime_error("Model file must be an .obj file\n");

    for (std::string line; getline(modelFile, line);) {

        std::string _;
        if (line.substr(0, line.find(' ')) == "v") {
            std::istringstream iss(line);

            float x, y, z;
            iss >> _ >> x >> y >> z;

            positions.push_back(glm::vec3(x, y, z));
        } else if (line.substr(0, line.find(' ')) == "vn") {
            std::istringstream iss(line);

            float x, y, z;
            iss >> _ >> x >> y >> z;

            normals.push_back(glm::vec3(x, y, z));
        } else if (line.substr(0, line.find(' ')) == "vt") {      
            std::istringstream iss(line);

            float u, v;
            iss >> _ >> u >> v;

            uvs.push_back(glm::vec2(u, v));
        } else if (line.substr(0, line.find(' ')) == "f") {
            if (util::strSpaces(line) != 3) throw std::runtime_error("Model faces must be traingulated\n");

            std::replace(line.begin(), line.end(), '/', ' ');
            std::istringstream iss(line);

            unsigned int positionIdx[3], normalIdx[3], uvIdx[3];
            iss >> _ >> positionIdx[0] >> uvIdx[0] >> normalIdx[0] >> positionIdx[1] >> uvIdx[1] >> normalIdx[1] >> positionIdx[2] >> uvIdx[2] >> normalIdx[2];

            positionIndices.insert(positionIndices.end(), { positionIdx[0], positionIdx[1], positionIdx[2] });
            uvIndices.insert(uvIndices.end(), { uvIdx[0], uvIdx[1], uvIdx[2] });
            normalIndices.insert(normalIndices.end(), { normalIdx[0], normalIdx[1], normalIdx[2] });
        }
    }

    for (size_t i = 0; i < positionIndices.size(); i++) {
        struct Vertex vertex = {
            .position = positions[positionIndices[i] - 1],
            .uv = uvs[uvIndices[i] - 1],
            .normal = normals[normalIndices[i] - 1]
        };

        vertices.push_back(vertex);
    }

    vertexCount = vertices.size();
}

void Model::bind() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);   
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(struct Vertex), &vertices[0], GL_STATIC_DRAW);  

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void*) 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void*) offsetof(struct Vertex, uv));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void*) offsetof(struct Vertex, normal));
}

Model::Model(const std::string& src) {
    loadData(src);
    bind();
}

Model::Model(const std::string& src, size_t instanceCount) {
    loadData(src);
    bind();

    glGenBuffers(1, &instanceVBO);

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, instanceCount * sizeof(InstanceData), nullptr, GL_DYNAMIC_DRAW);
    glBindVertexArray(VAO);

    for (int i = 0; i < 4; i++) {
        glEnableVertexAttribArray(3 + i);
        glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*) (offsetof(InstanceData, modelMatrix) + sizeof(glm::vec4) * i));
        glVertexAttribDivisor(3 + i, 1);
    }

    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*) offsetof(InstanceData, color));
    glVertexAttribDivisor(7, 1);

    glBindVertexArray(0);
}

void Model::updateInstanceData(size_t count, std::vector<glm::mat4>& matrices, std::vector<glm::vec4>& colors) {
    std::vector<InstanceData> instanceData(count);

    for (size_t i = 0; i < count; ++i) {
        instanceData[i].modelMatrix = matrices[i];
        instanceData[i].color = colors[i];
    }

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(InstanceData), instanceData.data());
}

Model::~Model() {
    // Uncommenting these two lines causes a diabolical segfault with the client (probably forgetting rule of five)
    /*glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);*/
}

void Model::render(Shader& shader, glm::mat4 model) {
    shader.use();

    shader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}

void Model::renderInstanced(Shader& shader, size_t count) {
    shader.use();

    glBindVertexArray(VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, vertexCount, count);
    glBindVertexArray(0);
}