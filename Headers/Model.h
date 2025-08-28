#pragma once

#include <Mesh.h>
#include <Texture.h>
#include <Shader.h>
#include <assimp/Importer.hpp>
#include <assimp/material.h>

class Model {
    struct MeshTexture{
        std::vector<std::pair<int, Texture>> m_Textures;
        Mesh m_Mesh;
        MeshTexture() = default;
        MeshTexture(const std::vector<Mesh::SizeDataPair>& attributes, std::vector<unsigned int> indices, std::vector<std::pair<int,Texture>> textures)
            : m_Mesh(attributes, indices), m_Textures(textures) {}
        void draw(Shader& shader) const;
        ~MeshTexture() = default;
    };
public:
    Model(const std::string& filepath);
    ~Model();
    void draw(Shader& shader) const;
    static void setMaterialName(const std::string& uniform);
    static void setDiffuseName(const std::string& uniform);
    static void setSpecularName(const std::string& uniform);
private:
    void loadModel(const std::string& filepath);
    void processNode(aiNode* node, const aiScene* scene);
    MeshTexture* processMesh(aiMesh* mesh, const aiScene* scene) const;
    std::vector<std::pair<int, Texture>> processMaterialTextrues(aiMaterial* material, aiTextureType type) const;
private:
    std::vector<MeshTexture*> m_TextureMesh;
    std::string m_Directory;
    static std::string s_MaterialName;
    static std::string s_DiffuseName;
    static std::string s_SpecularName;
};