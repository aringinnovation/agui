#include "agui/utils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace aring::agui {

bool Utils::LoadTextureFromFile(const char *filename, AGuiTexture *image) {
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

  image->im_texture_id = ( void * ) ( intptr_t ) index;
  image->width = static_cast< float >(width);
  image->height = static_cast< float >(height);

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
void Utils::DebugMetrics() {
  auto &res = ImGui::GetWindowViewport()->WorkSize;
  auto status = std::string(std::to_string(( int ) round(ImGui::GetIO().Framerate))).append("fps");
  ImGui::GetBackgroundDrawList()->AddText(ImGui::GetIO().Fonts->Fonts[0], 13.0f, ImVec2(res.x - status.length() * 8, res.y - 20), ImColor(100, 100, 100), status.c_str());
}
bool Utils::AddFontAwesome(const char *filename, float size) {
  ImFontConfig config;
  config.MergeMode = true;
  config.GlyphMinAdvanceX = size;
  static const ImWchar icon_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
  return ImGui::GetIO().Fonts->AddFontFromFileTTF(filename, size, &config, icon_ranges) != nullptr;
}
}// namespace aring::agui