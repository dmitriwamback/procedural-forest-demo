//
// Created by Dmitri Wamback on 2026-02-01.
//

#ifndef PROCEDURAL_FORESTS_TEXTURE_H
#define PROCEDURAL_FORESTS_TEXTURE_H

class Texture {
public:
    uint32_t textureID;

    static Texture CreateTexture(std::string texturePath, bool flip = true);
    void Bind();
};

inline Texture Texture::CreateTexture(std::string texturePath, bool flip) {

    Texture texture = Texture();

    stbi_set_flip_vertically_on_load(flip);

    int width, height, channels;
    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &channels, 0);

    glGenTextures(1, &texture.textureID);
    glBindTexture(GL_TEXTURE_2D, texture.textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

inline void Texture::Bind() {
    glBindTexture(GL_TEXTURE_2D, textureID);
}

#endif //PROCEDURAL_FORESTS_TEXTURE_H