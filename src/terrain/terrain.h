//
// Created by Dmitri Wamback on 2026-02-01.
//

#ifndef PROCEDURAL_FORESTS_TERRAIN_H
#define PROCEDURAL_FORESTS_TERRAIN_H
#include "noise.h"

#define TERRAIN_SIZE 5.0

class Terrain {
public:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    float heights[17][17];

    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 color = glm::vec3(0.6f, 0.0f, 0.0f);

    static Terrain CreateTerrain(int width, int height, glm::vec2 offset);

    void Render(Shader shader);

    float BaryCentric(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec2 p);
    float GetHeightAt(float wx, float wz);

private:
    uint32_t vertexArrayObject = 0, vertexBufferObject = 0, indexArrayObject = 0;
};

inline Terrain Terrain::CreateTerrain(int width, int height, glm::vec2 offset) {

    Terrain terrain = Terrain();

    std::vector<Vertex> vertices = {};
    std::vector<uint32_t> indices = {};

    for (int x = 0; x <= width; x++) {
        for (int z = 0; z <= height; z++) {

            Vertex vertex = {};

            float xC = ((float)(x + 0.3279235f) + offset.x);
            float zC = ((float)(z + 0.3279235f) + offset.y);

            float h = noiseLayer(xC / 20.4234f, zC / 20.4234f, 2.2f, 0.5f, 10, 0.241234f) * TERRAIN_SIZE/5.0f;
            terrain.heights[x][z] = h;

            vertex.position = glm::vec3(xC * TERRAIN_SIZE, h, zC * TERRAIN_SIZE);
            vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);
            vertex.uv = glm::vec2((xC - offset.x)/(float)width, (zC - offset.y)/(float)height) * 10.0f;

            vertices.push_back(vertex);
        }
    }

    for (int x = 0; x < width; x++) {
        for (int z = 0; z < height; z++) {

            uint32_t v0 = x * (height + 1) + z;
            uint32_t v1 = (x + 1) * (height + 1) + z;
            uint32_t v2 = x * (height + 1) + z + 1;
            uint32_t v3 = (x + 1) * (height + 1) + z + 1;

            indices.push_back(v0);
            indices.push_back(v1);
            indices.push_back(v2);

            indices.push_back(v1);
            indices.push_back(v3);
            indices.push_back(v2);
        }
    }

    terrain.vertices = vertices;
    terrain.indices = indices;

    for (auto &v : terrain.vertices) {
        v.normal = glm::vec3(0.0f);
    }

    for (size_t i = 0; i < terrain.indices.size(); i += 3) {
        uint32_t idx0 = terrain.indices[i + 0];
        uint32_t idx1 = terrain.indices[i + 1];
        uint32_t idx2 = terrain.indices[i + 2];

        glm::vec3 p0 = terrain.vertices[idx0].position;
        glm::vec3 p1 = terrain.vertices[idx1].position;
        glm::vec3 p2 = terrain.vertices[idx2].position;

        glm::vec3 faceNormal = glm::normalize(glm::cross(p1 - p0, p2 - p0));
        faceNormal.y = -faceNormal.y;

        terrain.vertices[idx0].normal += faceNormal;
        terrain.vertices[idx1].normal += faceNormal;
        terrain.vertices[idx2].normal += faceNormal;
    }

    for (auto &v : terrain.vertices) {
        v.normal = glm::normalize(v.normal);
    }


    glGenVertexArrays(1, &terrain.vertexArrayObject);

    glGenBuffers(1, &terrain.vertexBufferObject);
    glGenBuffers(1, &terrain.indexArrayObject);

    glBindVertexArray(terrain.vertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, terrain.vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, terrain.vertices.size() * sizeof(Vertex), &terrain.vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain.indexArrayObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, terrain.indices.size() * sizeof(uint32_t), &terrain.indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    glBindVertexArray(0);
    return terrain;
}

inline void Terrain::Render(Shader shader) {

    shader.Use();

    glm::mat4 model = glm::mat4(1.0f);

    glBindVertexArray(vertexArrayObject);

    shader.SetMatrix4("model", model);
    shader.SetVector3("color", color);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

inline float Terrain::BaryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos) {

    float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
    float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
    float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;

    float l3 = 1.0f - l1 - l2;
    return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

inline float Terrain::GetHeightAt(float wx, float wz) {
    float terrainX = wx;
    float terrainZ = wz;

    float gridSquare = TERRAIN_SIZE;
    int gridX = (int)floor(terrainX / gridSquare);
    int gridZ = (int)floor(terrainZ / gridSquare);

    float xCoord = (fmod(terrainX, gridSquare))/gridSquare;
    float zCoord = (fmod(terrainZ, gridSquare))/gridSquare;
    float result = 0;

    if (xCoord <= (1-zCoord)) {
        result = BaryCentric(glm::vec3(0, heights[gridX  ][gridZ  ], 0),
                             glm::vec3(1, heights[gridX+1][gridZ  ], 0),
                             glm::vec3(0, heights[gridX  ][gridZ+1], 1),
                             glm::vec2(xCoord, zCoord));
    }
    else {
        result = BaryCentric(glm::vec3(1, heights[gridX+1][gridZ  ], 0),
                             glm::vec3(1, heights[gridX+1][gridZ+1], 1),
                             glm::vec3(0, heights[gridX  ][gridZ+1], 1),
                             glm::vec2(xCoord, zCoord));
    }
    return result + 8;
}

#endif //PROCEDURAL_FORESTS_TERRAIN_H