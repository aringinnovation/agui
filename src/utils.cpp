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
  auto draw_list = ImGui::GetForegroundDrawList();

  auto status = std::string(std::to_string(( int ) round(ImGui::GetIO().Framerate))).append("fps");
  auto resolution = std::string(std::to_string(( int ) res.x)).append("x").append(std::to_string(( int ) res.y));

  float fps_start = res.x - static_cast< float >((status.length() * 9));
  float resolution_start = fps_start - static_cast< float >((resolution.length() * 8));

  draw_list->AddText(ImGui::GetIO().Fonts->Fonts[0], 13.0f, ImVec2(fps_start, 8), ImColor(100, 100, 100), status.c_str());
  draw_list->AddText(ImGui::GetIO().Fonts->Fonts[0], 13.0f, ImVec2(resolution_start, 8), ImColor(100, 100, 100), resolution.c_str());
}
bool Utils::AddFontAwesome(const char *filename, float size) {
  ImFontConfig config;
  config.MergeMode = true;
  config.GlyphMinAdvanceX = size;
  static const ImWchar icon_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
  return ImGui::GetIO().Fonts->AddFontFromFileTTF(filename, size, &config, icon_ranges) != nullptr;
}
void Utils::GradientBackground(ImVec4 from, ImVec4 to) {
  //void mygl_GradientBackground(float top_r, float top_g, float top_b, float top_a,
  //                             float bot_r, float bot_g, float bot_b, float bot_a) {
  glDisable(GL_DEPTH_TEST);

#define SHADER_HEADER "#version 330 core\n"
#define SHADER_STR(x) #x

  static GLuint background_vao = 0;
  static GLuint background_shader = 0;

  if (background_vao == 0) {

    glGenVertexArrays(1, &background_vao);

    const char *vs_src = ( const char * ) SHADER_HEADER SHADER_STR(
        out vec2 v_uv;
        void main() {
          uint idx = uint(gl_VertexID);
          gl_Position = vec4(idx & 1U, idx >> 1U, 0.0, 0.5) * 4.0 - 1.0;
          v_uv = vec2(gl_Position.xy * 0.5 + 0.5);
        });

    const char *fs_src = ( const char * ) SHADER_HEADER SHADER_STR(
        uniform vec4 top_color;
        uniform vec4 bot_color;
        in vec2 v_uv;
        out vec4 frag_color;

        void main() {
          frag_color = bot_color * (1 - v_uv.y) + top_color * v_uv.y;
        });
    GLuint vs_id, fs_id;
    vs_id = glCreateShader(GL_VERTEX_SHADER);
    fs_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vs_id, 1, &vs_src, NULL);
    glShaderSource(fs_id, 1, &fs_src, NULL);
    glCompileShader(vs_id);
    glCompileShader(fs_id);
    background_shader = glCreateProgram();
    glAttachShader(background_shader, vs_id);
    glAttachShader(background_shader, fs_id);
    glLinkProgram(background_shader);
    glDetachShader(background_shader, fs_id);
    glDetachShader(background_shader, vs_id);
    glDeleteShader(fs_id);
    glDeleteShader(vs_id);
    glUseProgram(background_shader);
  }

  glUseProgram(background_shader);
  GLuint top_color_loc = glGetUniformLocation(background_shader, "top_color");
  GLuint bot_color_loc = glGetUniformLocation(background_shader, "bot_color");
  glUniform4f(top_color_loc, top_r, top_g, top_b, top_a);
  glUniform4f(bot_color_loc, bot_r, bot_g, bot_b, bot_a);

  glBindVertexArray(background_vao);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glBindVertexArray(0);

  glEnable(GL_DEPTH_TEST);
  // }
}
}// namespace aring::agui