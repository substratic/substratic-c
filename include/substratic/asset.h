#ifndef _SUBSTRATIC_ASSET_H
#define _SUBSTRATIC_ASSET_H

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

typedef struct {
  int id;
  int width;
  int height;
} StcTexture;

StcTexture subst_asset_texture_load(const char *imagePath) {
  int width, height, nrChannels;
  unsigned char *data = stbi_load(imagePath, &width, &height, &nrChannels, STBI_rgb_alpha);

  // Load a texture
  unsigned int textureId;
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glBindTexture(GL_TEXTURE_2D, 0);

  StcTexture texture = {textureId, width, height};
  return texture;
}

#endif
