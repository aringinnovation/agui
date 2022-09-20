#ifndef ARING_AGUI_HELPER_
#define ARING_AGUI_HELPER_

#include <GLFW/glfw3.h>
#include <imgui.h>

namespace aring::agui {

class Utils {
 public:
  static bool LoadTextureFromFile(const char *filename, ImTextureID *image);
  static bool LoadImageFromFile(const char* filename, GLFWimage* output);
};
}// namespace aring::agui

#endif//ARING_AGUI_HELPER_
