#include <agui/agui.hpp>

namespace aring {

AGui::AGui(const std::string &application_title, int width, int height) {
  // Setup window
  glfwSetErrorCallback(glfw_error_callback);
  if (glfwInit() == 0) { return; }

  // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
  // GL ES 2.0 + GLSL 100
  const char *glsl_version = "#version 100";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
  // GL 3.2 + GLSL 150
  const char *glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);// 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);          // Required on Mac
#else
  // GL 3.0 + GLSL 130
  const char *glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif
  // Remove window decorations
  //glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

  // Create window with graphics context ( set glfwGetPrimaryMonitor() as monitor to fullscreen)
  this->window = glfwCreateWindow(width, height, application_title.c_str(), nullptr, nullptr);

  if (window == nullptr) { return; }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);// Enable vsync

  // Center window
  const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  glfwSetWindowPos(this->window, (mode->width / 2) - (width / 2), (mode->height / 2) - (height / 2));

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Add all built in fonts
  ImGui::GetIO().Fonts->AddFontDefault();
}
AGui::~AGui() {

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(window);
  glfwTerminate();
}
void AGui::Run() {

  StartUp();

  auto &gui_io = ImGui::GetIO();
  ( void ) gui_io;
  auto &style = ImGui::GetStyle();

  if ((gui_io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0) {
    style.WindowRounding = 0.0F;
    //style.Colors[ImGuiCol_WindowBg].w = 1.0F;
  }

  while (glfwWindowShouldClose(window) == 0) {
    // Poll input events
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    Update();

    // Rendering
    ImGui::EndFrame();
    //ImGui::UpdatePlatformWindows();
    ImGui::Render();
    int display_w;
    int display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0, 0, 0, 0);// Wipe background
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if ((gui_io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0) {
      GLFWwindow *backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup_current_context);
    }

    glfwSwapBuffers(window);
  }
}
GLFWwindow *AGui::Window() {
  return this->window;
}


}// namespace aring