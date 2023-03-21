#include "gui.hpp"

GUI::GUI(GLFWwindow *window) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");
}

void GUI::newFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void GUI::render(float &persistence, float &lacunarity, int &scale, int &octaves,
            float &xOffset, float &zOffset, int &height) {
  ImGui::Begin("ImGui Window");
  ImGui::Text("Hello, World");
  ImGui::SliderFloat("Persistence", &persistence, 0.1f, 1.0f);
  ImGui::SliderFloat("Lacrunarity", &lacunarity, 0.5f, 5.0f);
  ImGui::SliderInt("Scale", &scale, 1, 25);
  ImGui::SliderInt("Octaves", &octaves, 1, 8);
  ImGui::SliderFloat("xOffset", &xOffset, -20.0f, 20.0f);
  ImGui::SliderFloat("zOffset", &zOffset, -20.0f, 20.0f);
  ImGui::SliderInt("height", &height, 1, 50);
  // ImGui::InputInt("Seed", &seed);
  ImGui::End();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::Delete(){

}

