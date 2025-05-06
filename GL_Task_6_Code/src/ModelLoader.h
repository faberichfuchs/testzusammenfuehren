#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <string>
#include <iostream>
#include <GL/glew.h>

#include "Shader.h"

// Struktur für Vertex-Daten
struct Vertex {
    float position[3];     // Position des Vertex
    float normal[3];       // Normalen des Vertex
    float texCoords[2];    // Texturkoordinaten
    float tangent[3];      // Tangente für die Texturkoordinaten
    float bitangent[3];    // Bitangente für die Texturkoordinaten
};

// Struktur für ein Mesh
struct Mesh {
    std::vector<Vertex> vertices;  // Alle Vertices des Meshes
    std::vector<unsigned int> indices;  // Alle Indices des Meshes

    GLuint VAO, VBO, EBO;  // OpenGL Bufferobjekte (VAO, VBO, EBO)
    GLuint textureID = 0;   // Textur-ID (0 = keine Textur)

    // Initialisiere Bufferdaten
    void setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // VBO für Vertex-Daten
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        // EBO für Indizes
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // Vertex-Attribute (Position, Normal, TexCoords, Tangent, Bitangent)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

        glBindVertexArray(0);
    }

    // Draw Methode für das Mesh
    void Draw(Shader& shader) {
        if (textureID != 0) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureID);
            shader.setUniform("diffuseTexture", 0);
        }
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};

class ModelLoader {
public:
    // Konstruktor
    ModelLoader(const std::string& path);

    // Laden des Modells
    void loadModel(const std::string& path);

    // Zugriff auf die geladenen Meshes
    const std::vector<Mesh>& getMeshes() const { return meshes; }
    std::string modelDirectory;

    // Draw Methode zum Rendern aller Meshes
    void Draw(Shader& shader);

private:
    std::vector<Mesh> meshes; // Alle geladenen Meshes

    // Hilfsfunktionen
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    static GLuint loadTextureFromFile(const std::string& filename);
};

#endif // MODELLOADER_H

