#include <Model.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>
#include <utils.h>

std::string Model::s_MaterialName = "";
std::string Model::s_DiffuseName = "diffuse";
std::string Model::s_SpecularName = "specular";

void Model::MeshTexture::draw(Shader &shader) const {
    int diffuseNr{}, specularNr{};
    for (int i = 0; i < m_Textures.size(); i++) {
        auto [type, texture] = m_Textures.at(i);
        switch (type) {
        case aiTextureType_DIFFUSE:
            shader.setTexture((s_MaterialName.empty() ? "" : s_MaterialName + ".") + s_DiffuseName + std::to_string(diffuseNr++), texture.bind(i));
            break;
        case aiTextureType_SPECULAR:
            shader.setTexture((s_MaterialName.empty() ? "" : s_MaterialName + ".") + s_SpecularName + std::to_string(specularNr++), texture.bind(i));
            break;
        }
    }
    m_Mesh.draw();
}

Model::Model(const std::string& filepath) {
    loadModel(asAbsolutePath(filepath));
}

Model::~Model() {
    for (auto tm : m_TextureMesh)
        delete tm;
    m_TextureMesh.clear();
}

void Model::draw(Shader &shader) const {
    shader.use();
    for (const auto& tm : m_TextureMesh)
        tm->draw(shader);
}

void Model::setMaterialName(const std::string& uniform) {
    s_MaterialName = uniform;
}

void Model::setDiffuseName(const std::string& uniform) {
    s_DiffuseName = uniform;
}

void Model::setSpecularName(const std::string& uniform) {
    s_SpecularName = uniform;
}

void Model::loadModel(const std::string& filepath) {
    m_Directory = filepath.substr(0, filepath.find_last_of('/'));
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Failed to load model. " << import.GetErrorString() << std::endl;
        exit(EXIT_FAILURE);
    }
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    static int count = 1;
    count++;
    for (int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_TextureMesh.push_back(processMesh(mesh, scene));
    }
    for (int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Model::MeshTexture* Model::processMesh(aiMesh* mesh, const aiScene* scene) const {
    std::vector<float> postma;
    std::vector<float> cordtma;
    std::vector<float> normtma;
    std::vector<unsigned int> indices;
    aiVector3D tma;
    for (int i = 0; i < mesh->mNumVertices; i++) {
        tma = mesh->mVertices[i];
        postma.push_back(tma.x);
        postma.push_back(tma.y);
        postma.push_back(tma.z);
        tma = mesh->mNormals[i];
        normtma.push_back(tma.x);
        normtma.push_back(tma.y);
        normtma.push_back(tma.z);
        if (mesh->mTextureCoords[0]) {
            tma = mesh->mTextureCoords[0][i];
            cordtma.push_back(tma.x);
            cordtma.push_back(tma.y);
        } else {
            cordtma.push_back(0);
            cordtma.push_back(0);
        }
    }
    for (int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    std::vector<std::pair<int, Texture>> textures;
    if (scene->HasMaterials()) {
        auto diffuseTextures = processMaterialTextrues(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE);
        textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());
        auto specularTextures = processMaterialTextrues(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_SPECULAR);
        textures.insert(textures.end(), specularTextures.begin(), specularTextures.end());
    }
    return new MeshTexture({{3, postma}, {2, cordtma}, {3, normtma}}, indices, textures);
}

std::vector<std::pair<int, Texture>> Model::processMaterialTextrues(aiMaterial* material, aiTextureType type) const {
    std::vector<std::pair<int, Texture>> textures;
    for (int i = 0; i < material->GetTextureCount(type); i++) {
        aiString str;
        material->GetTexture(type, i, &str);
        Texture texture = Texture(m_Directory + str.C_Str());
        textures.push_back(std::make_pair(type, texture));
    }
    return textures;
}
