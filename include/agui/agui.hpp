#ifndef ARING_AGUI_MAIN_
#define ARING_AGUI_MAIN_

#include <GLFW/glfw3.h>
#include <cstdio>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#include <iostream>
#include <string>
#include <agui/style.hpp>

namespace aring {

static void glfw_error_callback(int error, const char *description) {
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

class AGui {
 public:
  explicit AGui(const std::string &application_title, int width, int height);
  ~AGui();
  void Run();

  virtual void Update(){};
  virtual void StartUp(){};

  GLFWwindow* Window();
 private:
  GLFWwindow *window{};
};

}// namespace aring

#endif//ARING_AGUI_MAIN_
