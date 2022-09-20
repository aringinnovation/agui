#include <agui/agui.hpp>
#include <agui/helper.hpp>
#include <vector>

class MyApp : public aring::AGui {
 public:
  MyApp() : aring::AGui("My test-app", 1280, 720) {}

  void Update() override {
    ImGui::ShowDemoWindow();

    ImGui::Image(this->images[0], ImVec2(512, 512));
    auto draw = ImGui::GetBackgroundDrawList();
    auto drawf = ImGui::GetForegroundDrawList();

    // nothing shows?
    draw->AddImage(this->images[0], ImVec2(600,600), ImVec2(100,100),ImVec2(0, 0), ImVec2(1, 1));
    draw->AddText(ImVec2(100, 100), ImColor(255, 0, 0), "Status Background");
    drawf->AddText(ImVec2(120, 120), ImColor(255, 0, 0), "Status Foreground");
  }

  void StartUp() override {
    auto &gui_io = ImGui::GetIO();

    DarkStyle(); // Load dark style

    // Set application icon
    auto icon = GLFWimage();
    if (!aring::agui::Utils::LoadImageFromFile("img/icon.png", &icon)) {
      throw std::runtime_error("Could not load icon file");
    }
    glfwSetWindowIcon(this->Window(), 1, &icon);

    // Set options
    gui_io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    gui_io.ConfigDockingWithShift = true;
    gui_io.ConfigDockingTransparentPayload = true;

    // Load fonts
    gui_io.Fonts->AddFontFromFileTTF("fonts/Roboto-Medium.ttf", 14.0F);
    gui_io.Fonts->AddFontFromFileTTF("fonts/JetBrainsMono-Regular.ttf", 12.0F);

    // Load background as texture
    ImTextureID img;
    bool result = aring::agui::Utils::LoadTextureFromFile("img/background.png", &img);
    assert(result);
    this->images.push_back(img);
  }

 private:
  std::vector< ImTextureID > images;
};

int main() {

  MyApp app;
  app.Run();

  return 0;
}
