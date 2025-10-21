#include "EvolutionApp.hpp"

#include <imgui.h>

#include <genesis/core/AppContext.hpp>
#include <genesis/core/Time.hpp>
#include <genesis/core/Window.hpp>
#include <genesis/model/Agent.hpp>
#include <genesis/model/MovementBehavior.hpp>
#include <genesis/model/SimpleEmbodiment.hpp>
#include <genesis/ui/ImGuiLayer.hpp>
#include <iostream>
#include <memory>

#include "genesis/core/Math.hpp"

using namespace gen;
using namespace std;

std::string EvolutionApp::Name() const {
    return "EvolutionApp";
}

void EvolutionApp::CreateAgent(const glm::vec2& pos, const glm::vec2& target) {
    auto agent = std::make_unique<Agent>("0");

    auto embodiment = std::make_unique<SimpleEmbodiment>(pos);
    agent->SetEmbodiment(std::move(embodiment));

    std::vector<int> layer{4, 8, 2};
    auto cap = std::make_unique<Capability>("movement", layer, std::make_unique<MovementBehavior>(target));
    cap->Randomize();
    agent->AddCapability(std::move(cap));
    m_Agents.emplace_back(std::move(agent));
}

bool EvolutionApp::Init(gen::AppContext& ctx) {
    std::cout << "Initializing EvolutionApp..." << std::endl;

    auto height = 100;

    m_Camera.SetOrthoByHeight(height, ctx.Aspect());

    m_AgentView.Init();

    m_Gui = std::make_unique<ImGuiLayer>(ctx.GetWindow().GetNativeWindow());

    onKeyPressed = [this](int key, int /*mods*/) {
        if (key == GLFW_KEY_ESCAPE) {
            m_Quit = true;
        }
    };

    onScroll = [this, &ctx](double /*xoffs*/, double yoffs) {
        m_Camera.ZoomAtCursor(yoffs * 0.1f, m_MousePos, ctx.GetWidth(), ctx.GetHeight());
    };

    onMouseMove = [this](double x, double y) {
        auto pos = glm::vec2(x, y);  //
        if (m_IsDragging) {
            glm::vec2 delta = pos - m_MousePos;
            delta.y *= -1.0;
            m_Camera.Pan(-delta * 0.2f);
        }
        m_MousePos = pos;
    };

    onMouseButton = [this](int button, int action, int /*mod*/) {
        if (button == GLFW_MOUSE_BUTTON_1) {
            if (action == GLFW_PRESS) {
                m_DragStart = m_MousePos;
                m_IsDragging = true;
            } else if (action == GLFW_RELEASE) {
                m_IsDragging = false;
            }
        }
    };

    onWindowSize = [this](int w, int h) {
        m_Camera.SetOrthoByHeight(h, float(w) / float(h));  //
    };

    std::cout << "EvolutionApp initialized." << std::endl;
    return true;
}

void EvolutionApp::Update(gen::AppContext& /*ctx*/, double dt) {
    for (auto& a : m_Agents) {
        a->Update(dt);
    }
    m_AgentView.UpdateInstances(m_Agents);
    m_Camera.Update(dt);
}

void EvolutionApp::Render(gen::AppContext& ctx) {
    if (m_Quit) {
        ctx.RequestQuit();
        return;
    }

    m_AgentView.Draw(m_Camera.ViewProj());

    m_Gui->BeginFrame();

    ImGui::Begin("Evolution Stats");
    ImGui::Separator();
    ImGui::SliderFloat("Timescale", &m_Timescale, 0.2f, 10.0f);
    ImGui::SliderInt("Agents", &m_NumAgents, 10, 100);
    if (ImGui::Button("Generate")) {
        m_Agents.clear();
        for (int i = 0; i < m_NumAgents; i++) {
            auto pos = RandUnitVec2();
            pos *= 50;
            CreateAgent(pos, {0, 0});
        }
    }
    ImGui::End();

    m_Gui->EndFrame();
}
