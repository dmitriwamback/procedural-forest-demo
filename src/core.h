//
// Created by Dmitri Wamback on 2026-02-01.
//

#ifndef PROCEDURAL_FORESTS_CORE_H
#define PROCEDURAL_FORESTS_CORE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

inline GLFWwindow *window;

#include <unordered_map>
#include <tuple>

#include <glm/glm/vec2.hpp>
#include <glm/glm/vec3.hpp>
#include <glm/glm/mat4x4.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include <vector>

#include "camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "texture/stb_image.h"
#include "texture/texture.h"

#include "shader/shader.h"
#include "vertex.h"
#include "terrain/cube.h"
#include "terrain/terrain.h"

inline Terrain terrain;

inline Texture terrainTexture;

struct Chunk {
    int x, z;
    Terrain terrain;
};

struct TupleHash {
    std::size_t operator()(const std::tuple<int,int>& key) const {
        size_t h1 = std::hash<int>{}(std::get<0>(key));
        size_t h2 = std::hash<int>{}(std::get<1>(key));
        return h1 ^ (h2 << 1); // simple combination
    }
};

std::unordered_map<std::tuple<int,int>, Chunk, TupleHash> loadedChunks;
int chunkRenderDistance = 15; // chunks around camera
int chunkSize = 16;

inline void initialize() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(1200, 800, "procedural forest", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        std::exit(EXIT_FAILURE);
    }

    Camera::Initialize();
    Shader shader = Shader::CreateShader("/Users/dmitriwamback/CLionProjects/procedural-forests/src/shader/sources/main");
    terrainTexture = Texture::CreateTexture("/Users/dmitriwamback/CLionProjects/procedural-forests/src/res/grass2.png");

    glfwSetCursorPosCallback(window, cursor_position_callback);

    Cube cube = Cube::CreateCube();

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

        // --- Input ---
        glm::vec4 movement(0.0f);
        movement.x = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ? 1.0f : 0.0f;
        movement.y = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ? -1.0f : 0.0f;
        movement.z = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ? 1.0f : 0.0f;
        movement.w = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ? -1.0f : 0.0f;

        float up   = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS ? 1.0f : 0.0f;
        float down = glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS ? -1.0f : 0.0f;

        camera.Update(movement, up, down);

        // --- Determine which chunk camera is in ---
        float chunkWorldSize = chunkSize * TERRAIN_SIZE;
        int camChunkX = (int)floor(camera.position.x / chunkWorldSize);
        int camChunkZ = (int)floor(camera.position.z / chunkWorldSize);

        terrainTexture.Bind();

        // --- Load nearby chunks first ---
        for (int x = camChunkX - chunkRenderDistance; x <= camChunkX + chunkRenderDistance; x++) {
            for (int z = camChunkZ - chunkRenderDistance; z <= camChunkZ + chunkRenderDistance; z++) {
                auto key = std::make_tuple(x,z);
                if (loadedChunks.find(key) == loadedChunks.end()) {
                    Chunk chunk;
                    chunk.x = x; chunk.z = z;
                    chunk.terrain = Terrain::CreateTerrain(chunkSize, chunkSize, glm::vec2(x*chunkSize, z*chunkSize));
                    loadedChunks[key] = chunk;
                }
            }
        }

        // --- Remove far chunks ---
        std::vector<std::tuple<int,int>> chunksToRemove;
        for (auto& [key, chunk] : loadedChunks) {
            int dx = std::get<0>(key) - camChunkX;
            int dz = std::get<1>(key) - camChunkZ;
            if (abs(dx) > chunkRenderDistance || abs(dz) > chunkRenderDistance) {
                chunksToRemove.push_back(key);
            }
        }
        for (auto& key : chunksToRemove) loadedChunks.erase(key);

        // --- Update camera Y based on terrain ---
        auto key = std::make_tuple(camChunkX, camChunkZ);
        if (loadedChunks.find(key) != loadedChunks.end()) {
            Terrain &currentChunk = loadedChunks[key].terrain;

            float localX = camera.position.x - camChunkX * chunkWorldSize;
            float localZ = camera.position.z - camChunkZ * chunkWorldSize;

            // clamp coordinates inside chunk
            localX = glm::clamp(localX, 0.0f, chunkWorldSize);
            localZ = glm::clamp(localZ, 0.0f, chunkWorldSize);

            float playerHeightOffset = 2.0f;
            //camera.position.y = currentChunk.GetHeightAt(localX, localZ) + playerHeightOffset;
        }

        camera.UpdateLookAtMatrix();

        // --- Render all chunks ---
        shader.Use();
        shader.SetMatrix4("projection", camera.projection);
        shader.SetMatrix4("lookAt", camera.lookAt);
        for (auto& [key, chunk] : loadedChunks) {
            chunk.terrain.Render(shader);
        }

        cube.Render(shader);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

#endif //PROCEDURAL_FORESTS_CORE_H