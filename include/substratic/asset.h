#ifndef _SUBSTRATIC_ASSET_H
#define _SUBSTRATIC_ASSET_H

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

typedef struct {
  int width;
  int height;
  unsigned char *data;
} StcImage;

StcImage subst_asset_load_image(const char *imagePath) {
  int width, height, nrChannels;
  unsigned char *data = stbi_load(imagePath, &width, &height, &nrChannels, STBI_rgb_alpha);

  StcImage image = {width, height, data};
  return image;
}

#endif
