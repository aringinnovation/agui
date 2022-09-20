#include "agui/helper.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace aring::agui {

bool Utils::LoadTextureFromFile(const char *filename, ImTextureID *image) {
  int width, height;
  GLuint index;
  unsigned char *image_data = stbi_load(filename, &width, &height, nullptr, 4);
  if (image_data == nullptr) { return false; }

  glGenTextures(1, &index);
  glBindTexture(GL_TEXTURE_2D, index);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                  GL_CLAMP_TO_EDGE);                                  // This is required on WebGL for non power-of-two textures
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// Same

  // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
  stbi_image_free(image_data);

  *image = ( void * ) ( intptr_t ) index;
  return true;
}
bool Utils::LoadImageFromFile(const char *filename, GLFWimage *output) {
  int width;
  int height;
  int nrChannels;
  unsigned char *img = stbi_load(filename, &width, &height, &nrChannels, 4);
  if (img == nullptr) return false;
  *output = GLFWimage{width, height, img};
  return true;
}
}// namespace aring::agui