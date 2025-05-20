#pragma once

#include <glad/glad.h>
#include <glm.hpp>
#include <vector>

#include "Shader.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    void Draw(Shader& shader);

private:
    unsigned int VBO, EBO;
    void setupMesh();
};



