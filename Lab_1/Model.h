#pragma once

#include <string>
#include <vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"

class Model {
public:
    // Загружаем модель из файла
    Model(const std::string& path);

    // Отрисовка модели
    void Draw(Shader& shader);

private:
    std::vector<Mesh> meshes;
    std::string directory;

    // Вспомогательные методы
    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
};


