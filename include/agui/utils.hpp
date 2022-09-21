#ifndef ARING_AGUI_HELPER_
#define ARING_AGUI_HELPER_

#include <GLFW/glfw3.h>
#include <agui/font_awesome.h>
#include <cmath>
#include <imgui.h>
#include <iostream>
#include <string>

namespace aring::agui {

struct AGuiTexture {
  ImTextureID im_texture_id;
  float width;
  float height;
};

class Utils {
 public:
  static bool LoadTextureFromFile(const char *filename, AGuiTexture *image);
  static bool LoadImageFromFile(const char *filename, GLFWimage *output);
  static void DebugMetrics();
  static bool AddFontAwesome(const char *filename, float size);
};
}// namespace aring::agui

#endif//ARING_AGUI_HELPER_
