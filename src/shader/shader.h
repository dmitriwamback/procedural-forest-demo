//
// Created by Dmitri Wamback on 2026-02-01.
//

#ifndef PROCEDURAL_FORESTS_SHADER_H
#define PROCEDURAL_FORESTS_SHADER_H
#include <string>
#include <fstream>
#include <sstream>

class Shader {
public:
    uint32_t program;
    static Shader CreateShader(const char* shaderPath);

    void SetMatrix4(const char* name, glm::mat4& mat) const;
    void SetVector3(const char* name, glm::vec3& vec) const;
    void Use() const;

private:
    static void CompileShader(int shader, const char* shaderSource);
    static void PrintShaderInfoLog(int shader);
    static int LoadShaderSource(const char* shaderPath, int shaderType);
};

inline Shader Shader::CreateShader(const char* shaderPath) {
    Shader shader = Shader();

    std::string vsSource = (std::string(shaderPath) + "/vMain.glsl");
    std::string fsSource = (std::string(shaderPath) + "/fMain.glsl");

    const char* vertexShaderPath = vsSource.c_str();
    const char* fragmentShaderPath = fsSource.c_str();

    int vert = Shader::LoadShaderSource(vertexShaderPath, GL_VERTEX_SHADER);
    int frag = Shader::LoadShaderSource(fragmentShaderPath, GL_FRAGMENT_SHADER);

    shader.program = glCreateProgram();
    glAttachShader(shader.program, vert);
    glAttachShader(shader.program, frag);
    glLinkProgram(shader.program);
    glDeleteShader(vert);
    glDeleteShader(frag);

    return shader;
}

inline int Shader::LoadShaderSource(const char* shaderPath, int shaderType) {

    std::ifstream shaderFile(shaderPath);
    std::stringstream ss;

    ss << shaderFile.rdbuf();
    shaderFile.close();

    std::string shaderS = ss.str();
    const char* shaderSource = shaderS.c_str();

    int shaderProgram = glCreateShader(shaderType);
    Shader::CompileShader(shaderProgram, shaderSource);
    Shader::PrintShaderInfoLog(shaderProgram);

    return shaderProgram;
}

inline void Shader::PrintShaderInfoLog(int shader) {

    int success;
    char infoLog[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << infoLog << std::endl;
    }
    else {
        std::cout << infoLog << std::endl;
    }
}

inline void Shader::CompileShader(int shader, const char* shaderSource) {
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);
}

inline void Shader::SetVector3(const char* name, glm::vec3& vec) const {
    glUniform3f(glGetUniformLocation(program, name), vec[0], vec[1], vec[2]);
}

inline void Shader::SetMatrix4(const char* name, glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, &mat[0][0]);
}

inline void Shader::Use() const {
    glUseProgram(program);
}

#endif //PROCEDURAL_FORESTS_SHADER_H