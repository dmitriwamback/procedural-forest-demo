//
// Created by Dmitri Wamback on 2026-02-01.
//

#ifndef PROCEDURAL_FORESTS_VERTEX_H
#define PROCEDURAL_FORESTS_VERTEX_H

typedef struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;

    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv) {
        this->position = position;
        this->normal = normal;
        this->uv = uv;
    }

    Vertex(glm::vec3 position) {
        this->position = position;
    }

    Vertex() {}
};

#endif //PROCEDURAL_FORESTS_VERTEX_H