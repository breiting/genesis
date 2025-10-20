#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <roboto_regular.h>

#include <genesis/ui/ImGuiLayer.hpp>

namespace gen {

ImGuiLayer::ImGuiLayer(GLFWwindow* window) : m_Window(window) {
    IMGUI_CHECKVERSION();
    m_Ctx = ImGui::CreateContext();
    ImGui::SetCurrentContext(m_Ctx);

    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    ImFontConfig fontConfig;
    fontConfig.FontDataOwnedByAtlas = false;
    ImFont* robotoFont =
        io.Fonts->AddFontFromMemoryTTF((void*)g_RobotoRegular, sizeof(g_RobotoRegular), 16.0f, &fontConfig);
    io.FontDefault = robotoFont;
}

ImGuiLayer::~ImGuiLayer() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(m_Ctx);
}

void ImGuiLayer::BeginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::Render() {
    // default empty; app can draw panels
}

void ImGuiLayer::EndFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Multi-viewport
    ImGuiIO& io = ImGui::GetIO();
}

}  // namespace gen
