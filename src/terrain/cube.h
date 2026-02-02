//
// Created by Dmitri Wamback on 2026-02-01.
//

#ifndef PROCEDURAL_FORESTS_CUBE_H
#define PROCEDURAL_FORESTS_CUBE_H

class Cube {
public:

    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 color = glm::vec3(0.6f, 0.0f, 0.0f);

    std::vector<Vertex> vertices = {
        Vertex({-1.0f, -1.0f, -1.0f},  {0, 0, -1}, {0, 0}),
        Vertex({ 1.0f,  1.0f, -1.0f},  {0, 0, -1}, {1, 1}),
        Vertex({ 1.0f, -1.0f, -1.0f},  {0, 0, -1}, {1, 0}),
        Vertex({ 1.0f,  1.0f, -1.0f},  {0, 0, -1}, {1, 1}),
        Vertex({-1.0f, -1.0f, -1.0f},  {0, 0, -1}, {0, 0}),
        Vertex({-1.0f,  1.0f, -1.0f},  {0, 0, -1}, {0, 1}),

        Vertex({-1.0f, -1.0f,  1.0f},  {0, 0, 1}, {0, 0}),
        Vertex({ 1.0f, -1.0f,  1.0f},  {0, 0, 1}, {1, 0}),
        Vertex({ 1.0f,  1.0f,  1.0f},  {0, 0, 1}, {1, 1}),
        Vertex({ 1.0f,  1.0f,  1.0f},  {0, 0, 1}, {1, 1}),
        Vertex({-1.0f,  1.0f,  1.0f},  {0, 0, 1}, {0, 1}),
        Vertex({-1.0f, -1.0f,  1.0f},  {0, 0, 1}, {0, 0}),

        Vertex({-1.0f,  1.0f,  1.0f},  {-1, 0, 0}, {1, 1}),
        Vertex({-1.0f,  1.0f, -1.0f},  {-1, 0, 0}, {1, 0}),
        Vertex({-1.0f, -1.0f, -1.0f},  {-1, 0, 0}, {0, 0}),
        Vertex({-1.0f, -1.0f, -1.0f},  {-1, 0, 0}, {0, 0}),
        Vertex({-1.0f, -1.0f,  1.0f},  {-1, 0, 0}, {0, 1}),
        Vertex({-1.0f,  1.0f,  1.0f},  {-1, 0, 0}, {1, 1}),

        Vertex({ 1.0f,  1.0f,  1.0f},  {1, 0, 0}, {1, 1}),
        Vertex({ 1.0f, -1.0f, -1.0f},  {1, 0, 0}, {0, 0}),
        Vertex({ 1.0f,  1.0f, -1.0f},  {1, 0, 0}, {1, 0}),
        Vertex({ 1.0f, -1.0f, -1.0f},  {1, 0, 0}, {0, 0}),
        Vertex({ 1.0f,  1.0f,  1.0f},  {1, 0, 0}, {1, 1}),
        Vertex({ 1.0f, -1.0f,  1.0f},  {1, 0, 0}, {0, 1}),

        Vertex({-1.0f, -1.0f, -1.0f},  {0, -1, 0}, {0, 0}),
        Vertex({ 1.0f, -1.0f, -1.0f},  {0, -1, 0}, {1, 0}),
        Vertex({ 1.0f, -1.0f,  1.0f},  {0, -1, 0}, {1, 1}),
        Vertex({ 1.0f, -1.0f,  1.0f},  {0, -1, 0}, {1, 1}),
        Vertex({-1.0f, -1.0f,  1.0f},  {0, -1, 0}, {0, 1}),
        Vertex({-1.0f, -1.0f, -1.0f},  {0, -1, 0}, {0, 0}),

        Vertex({-1.0f,  1.0f, -1.0f},  {0, 1, 0}, {0, 0}),
        Vertex({ 1.0f,  1.0f,  1.0f},  {0, 1, 0}, {1, 1}),
        Vertex({ 1.0f,  1.0f, -1.0f},  {0, 1, 0}, {1, 0}),
        Vertex({ 1.0f,  1.0f,  1.0f},  {0, 1, 0}, {1, 1}),
        Vertex({-1.0f,  1.0f, -1.0f},  {0, 1, 0}, {0, 0}),
        Vertex({-1.0f,  1.0f,  1.0f},  {0, 1, 0}, {0, 1}),
    };

    static Cube CreateCube();
    void Render(Shader shader);

private:
    uint32_t vertexArrayObject = 0, vertexBufferObject = 0, indexArrayObject = 0;
};

inline Cube Cube::CreateCube() {

    Cube cube = Cube();

    cube.position = glm::vec3(0.0f, 0.0f, 0.0f);
    cube.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    cube.scale = glm::vec3(1.0f, 1.0f, 1.0f);

    cube.color = glm::vec3(1.0f);

    glGenVertexArrays(1, &cube.vertexArrayObject);
    glBindVertexArray(cube.vertexArrayObject);

    glGenBuffers(1, &cube.vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, cube.vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, cube.vertices.size() * sizeof(Vertex), cube.vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    return cube;
}

inline void Cube::Render(Shader shader) {
    shader.Use();

    glm::mat4 model = glm::mat4(1.0f);

    glBindVertexArray(vertexArrayObject);

    shader.SetMatrix4("model", model);
    shader.SetVector3("color", color);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

#endif //PROCEDURAL_FORESTS_CUBE_H