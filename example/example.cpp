#include <agui/agui.hpp>
#include <agui/utils.hpp>
#include <vector>

class MyApp : public aring::AGui {
 public:
  MyApp() : aring::AGui("My test-app", 1280, 720) {}

  void Update() override {
    // Since default font is loaded at position 0 we need to use Push/Popfont to use
    // Roboto as our "main font"
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

    ImGui::ShowDemoWindow();

    auto draw = ImGui::GetBackgroundDrawList();
    auto drawf = ImGui::GetForegroundDrawList();

    auto &image = this->images[0];

    draw->AddImage(image.im_texture_id, ImVec2(image.width, image.height), ImVec2(0,0), ImVec2(1, 1), ImVec2(0, 0));
    draw->AddText(ImVec2(100, 100), ImColor(255, 0, 0), "Status Background");
    drawf->AddText(ImVec2(120, 120), ImColor(255, 0, 0), "Status Foreground");

    aring::agui::Utils::DebugMetrics();

    ImGui::Begin("Another Window");
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
    ImGui::Text("%s among %d items", ICON_FA_SEARCH, 12);
    ImGui::PopFont();
    ImGui::End();

    // Don't forget to popFont!
    ImGui::PopFont();
  }

  void StartUp() override {
    auto &gui_io = ImGui::GetIO();

    DarkStyle();// Load dark style

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
    gui_io.Fonts->AddFontFromFileTTF("fonts/Roboto-Medium.ttf", 14.0F);        //pos 1
    gui_io.Fonts->AddFontFromFileTTF("fonts/JetBrainsMono-Regular.ttf", 12.0F);// pos 2
    // Icon fonts
    aring::agui::Utils::AddFontAwesome("fonts/fontawesome-webfont.ttf", 12.0F);// pos 3

    // Load background as texture
    aring::agui::AGuiTexture img{};
    bool result = aring::agui::Utils::LoadTextureFromFile("img/background.png", &img);
    assert(result);
    this->images.push_back(img);
  }

 private:
  std::vector< aring::agui::AGuiTexture > images;
};

int main() {

  MyApp app;
  app.Run();

  return 0;
}
