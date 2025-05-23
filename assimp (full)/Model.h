#pragma once

#include <string>
#include <vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"

using namespace std;

class Model {
public:


    vector<Mesh> meshes;
    string directory;

    // Загружаем модель из файла
    Model(const std::string& path) {
        loadModel(path);
    };


    void Draw(Shader& shader);

    // Отрисовка модели
    void Draw(Shader& shader, glm::mat4 transform_x1, glm::mat4 transform_x2, glm::mat4 transform_x3) {
        for (unsigned int i = 0; i < meshes.size(); i++) {
            switch (i)
            {
            case 1: {
                shader.setMat4("model", transform_x1);
            }
                  break;
            case 2: {
                glm::mat4 minusTranslate_x2 = glm::translate(glm::mat4(1.0), glm::vec3(0.0, -2.2, 0.0));
                glm::mat4 Translate_x2 = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 2.2, 0.0));

                glm::mat4 result = transform_x1 * Translate_x2 * transform_x2 * minusTranslate_x2;
                shader.setMat4("model", result);

            }
                  break;
            case 3: {
                glm::mat4 minusTranslate_x3 = glm::translate(glm::mat4(1.0), glm::vec3(0.0, -5.2, 0.0));
                glm::mat4 Translate_x3 = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 5.2, 0.0));

                glm::mat4 rotationOnly_x2 = glm::mat4(glm::mat3(transform_x2)); // Извлекаем только вращение
                rotationOnly_x2[3] = transform_x2[3]; // Сохраняем позицию

                glm::mat4 result2 = transform_x1 * rotationOnly_x2 * Translate_x3 * transform_x3 * minusTranslate_x3;
                shader.setMat4("model", result2);
            }
                  break;
            default:
                shader.setMat4("model", glm::mat4(1.0));
                break;
            }
            meshes[i].Draw(shader);
        }
    };

private:


    // Вспомогательные методы
    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
};

