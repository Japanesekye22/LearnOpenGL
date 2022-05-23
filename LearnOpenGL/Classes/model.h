#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>


class Model
{
public:
    // constructor
    Model(std::string const& path);

    // draw every mesh in model
    void Draw(Shader& shader);

private:
    // mesh data
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;

    // load model into Assimp Scene object
    void loadModel(std::string const& path);

    // recursively process all nodes in scene
    void processNode(aiNode* node, const aiScene* scene);

    // process Assimp mesh data to our Mesh object
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    // load material textures
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

    // loads texture using stbi_image
    unsigned int TextureFromFile(const char* path, const std::string& directory);
};

#endif