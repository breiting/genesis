#include "EvolutionApp.hpp"

#include <imgui.h>

#include <genesis/core/AppContext.hpp>
#include <genesis/core/Time.hpp>
#include <genesis/core/Window.hpp>
#include <genesis/ui/ImGuiLayer.hpp>
#include <iostream>
#include <memory>

#include "genesis/modules/BrownianSim.hpp"
#include "genesis/modules/NeuroEvolutionSim.hpp"

using namespace gen;
using namespace std;

std::string EvolutionApp::Name() const {
    return "EvolutionApp";
}

bool EvolutionApp::Init(gen::AppContext& ctx) {
    std::cout << "Initializing EvolutionApp..." << std::endl;

    m_Gui = std::make_unique<ImGuiLayer>(ctx.GetWindow().GetNativeWindow());
    ApplyDarkStyle();

    // Viewports
    m_ViewportUi = {0, 0, ImGuiPanelWidth, ctx.GetHeight()};
    m_ViewportWorld = {ImGuiPanelWidth, 0, ctx.GetWidth() - ImGuiPanelWidth, ctx.GetHeight()};
    m_Camera.SetViewportSize(glm::vec2(m_ViewportWorld.w, m_ViewportWorld.h));
    m_Camera.SetPosition(glm::vec2{0.0f});
    m_Camera.FitTo(m_WorldSize);

    m_CanvasView.Init();

    onKeyPressed = [this](int key, int /*mods*/) {
        if (key == GLFW_KEY_ESCAPE) {
            m_Quit = true;
        } else if (key == GLFW_KEY_F) {
            m_Camera.FitTo(m_WorldSize);
            m_Camera.SetPosition(glm::vec2{0.0f});
        }
    };

    onScroll = [this](double /*xoffs*/, double yoffs) {
        if (!MouseInWorldVP(m_MousePos)) return;
        m_Camera.ZoomAtCursor(yoffs * 0.1f, m_MousePos);  //
    };

    onMouseMove = [this](double x, double y) {
        auto pos = glm::vec2(x, y);  //
        if (m_IsDragging && MouseInWorldVP(pos)) {
            glm::vec2 delta = pos - m_MousePos;
            delta.y *= -1.0;
            m_Camera.Pan(delta);
        }
        m_MousePos = pos;
    };

    onMouseButton = [this](int button, int action, int /*mod*/) {
        if (button == GLFW_MOUSE_BUTTON_1) {
            if (action == GLFW_PRESS) {
                if (!MouseInWorldVP(m_MousePos)) return;
                m_DragStart = m_MousePos;
                m_IsDragging = true;
            } else if (action == GLFW_RELEASE) {
                m_IsDragging = false;
            }
        }
    };

    onWindowSize = [this](int w, int h) {
        m_ViewportUi = {0, 0, ImGuiPanelWidth, h};
        m_ViewportWorld = {ImGuiPanelWidth, 0, w - ImGuiPanelWidth, h};
        m_Camera.SetViewportSize(glm::vec2(m_ViewportWorld.w, m_ViewportWorld.h));
        m_Camera.FitTo(m_WorldSize);
    };

    // Modules
    m_Modules.emplace_back(std::make_unique<NeuroEvolutionSim>());
    m_Modules.emplace_back(std::make_unique<BrownianSim>());
    m_CurrentModule = m_Modules.front().get();
    m_CurrentModule->OnAttach();

    std::cout << "EvolutionApp initialized." << std::endl;
    return true;
}

void EvolutionApp::Update(gen::AppContext& /*ctx*/, double dt) {
    if (m_CurrentModule) {
        m_CurrentModule->Update(dt * m_Timescale);
    }

    m_Camera.Update(dt);
}

bool EvolutionApp::MouseInWorldVP(const glm::vec2& mouse) {
    return mouse.x >= m_ViewportWorld.x && mouse.x < (m_ViewportWorld.x + m_ViewportWorld.w) &&
           mouse.y >= m_ViewportWorld.y && mouse.y < (m_ViewportWorld.y + m_ViewportWorld.h);
}
glm::vec2 EvolutionApp::ToLocalViewport(const glm::vec2& mouseScreen) {
    return {mouseScreen.x - m_ViewportWorld.x, mouseScreen.y - m_ViewportWorld.y};
}

glm::vec2 EvolutionApp::GetMousePosition() {
    return {m_MousePos.x - m_ViewportWorld.x, m_MousePos.y - m_ViewportWorld.y};
}

void EvolutionApp::Render(gen::AppContext& ctx) {
    if (m_Quit) {
        ctx.RequestQuit();
        return;
    }

    // Render CANVAS / World
    glEnable(GL_SCISSOR_TEST);
    glViewport(m_ViewportWorld.x, m_ViewportWorld.y, m_ViewportWorld.w, m_ViewportWorld.h);
    glScissor(m_ViewportWorld.x, m_ViewportWorld.y, m_ViewportWorld.w, m_ViewportWorld.h);
    glDisable(GL_SCISSOR_TEST);

    m_CanvasView.Draw(m_WorldSize, m_Camera.ViewProj());
    if (m_CurrentModule) {
        m_CurrentModule->RenderWorld(m_Camera.ViewProj());
    }

    // Render UI
    glViewport(0, 0, ctx.GetWidth(), ctx.GetHeight());
    glDisable(GL_SCISSOR_TEST);

    m_Gui->BeginFrame();
    DrawControlPanel();
    m_Gui->EndFrame();
}

void EvolutionApp::DrawControlPanel() {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(ImGuiPanelWidth, (float)ImGui::GetIO().DisplaySize.y));

    ImGui::Begin("Control Panel", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoTitleBar);

    ImGui::TextColored(ImVec4(0.6f, 0.9f, 0.8f, 1.0f), "GENESIS EVOLUTION");
    ImGui::Separator();
    if (ImGui::BeginCombo("Module", m_CurrentModule->Name().c_str())) {
        for (auto& m : m_Modules) {
            if (ImGui::Selectable(m->Name().c_str())) {
                m_CurrentModule->OnDetach();
                m_CurrentModule = m.get();
                m_CurrentModule->OnAttach();
            }
        }
        ImGui::EndCombo();
        ImGui::Separator();
    }
    ImGui::Spacing();

    if (m_CurrentModule) {
        m_CurrentModule->RenderImGui();
    }

    ImGui::End();
}

void EvolutionApp::ApplyDarkStyle() {
    auto& style = ImGui::GetStyle();
    ImGui::StyleColorsDark();

    style.WindowRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.ScrollbarRounding = 6.0f;

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.0f);
    colors[ImGuiCol_Header] = ImVec4(0.22f, 0.22f, 0.22f, 1.0f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
    colors[ImGuiCol_Button] = ImVec4(0.22f, 0.25f, 0.25f, 1.0f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.27f, 0.30f, 0.30f, 1.0f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.2f, 0.45f, 0.35f, 1.0f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
}
