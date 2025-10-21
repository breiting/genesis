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

#include "genesis/model/MovementEvaluator.hpp"

using namespace gen;
using namespace std;

std::string EvolutionApp::Name() const {
    return "EvolutionApp";
}

void EvolutionApp::CreateAgent(const glm::vec2& pos, const glm::vec2& target) {
    auto agent = std::make_unique<Agent>("0");

    auto embodiment = std::make_unique<SimpleEmbodiment>(pos);
    agent->SetEmbodiment(std::move(embodiment));

    std::vector<int> layer{5, 8, 2};
    auto cap = std::make_unique<Capability>("movement", layer, std::make_unique<MovementBehavior>(target));
    cap->Randomize();
    agent->AddCapability(std::move(cap));
    m_Agents.emplace_back(std::move(agent));
}

bool EvolutionApp::Init(gen::AppContext& ctx) {
    std::cout << "Initializing EvolutionApp..." << std::endl;

    m_StartPos = glm::vec2(-50.0f, 0.0f);
    m_TargetPos = glm::vec2(0.0f, 0.0f);

    m_Evaluator = std::make_unique<MovementEvaluator>(m_TargetPos);

    // Generate population
    for (int i = 0; i < m_NumAgents; i++) {
        CreateAgent(m_StartPos, m_TargetPos);
    }

    auto height = 100;

    m_Camera.SetOrthoByHeight(height, ctx.Aspect());

    m_AgentView.Init();
    m_StartView.Init();
    m_TargetView.Init();

    m_Gui = std::make_unique<ImGuiLayer>(ctx.GetWindow().GetNativeWindow());

    onKeyPressed = [this](int key, int /*mods*/) {
        if (key == GLFW_KEY_ESCAPE) {
            m_Quit = true;
        } else if (key == GLFW_KEY_E) {
            float fitness = m_Evaluator->Evaluate(*m_Agents[0].get());
            printf("FITNESS %f\n", fitness);
        } else if (key == GLFW_KEY_O) {
            m_IsObserving = !m_IsObserving;
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
    if (m_IsObserving) {
        for (auto& a : m_Agents) {
            a->Update(dt * m_Timescale);
        }
        m_AgentView.UpdateInstances(m_Agents);
    }
    m_Camera.Update(dt);
}

void EvolutionApp::RepositionAgents(bool random) {
    if (random) {
        for (int i = 0; i < m_NumAgents; i++) {
            glm::vec2 offset = RandUnitVec2() * RandFloat(0.0f, 5);
            m_Agents[i]->GetEmbodiment()->SetPosition(m_StartPos + offset);
            m_Agents[i]->GetEmbodiment()->SetVelocity(glm::vec2(0.0f));
        }
    } else {
        for (int i = 0; i < m_NumAgents; i++) {
            m_Agents[i]->GetEmbodiment()->SetPosition(m_StartPos);
            m_Agents[i]->GetEmbodiment()->SetVelocity(glm::vec2(0.0f));
        }
    }
}

void EvolutionApp::Render(gen::AppContext& ctx) {
    if (m_Quit) {
        ctx.RequestQuit();
        return;
    }

    m_StartView.Draw(m_StartPos, 5.0f, glm::vec4(1.0, 1.0, 0.0, 1.0), m_Camera.ViewProj());
    m_TargetView.Draw(m_TargetPos, 2.0f, glm::vec4(0.0, 1.0, 1.0, 1.0), m_Camera.ViewProj());
    m_AgentView.Draw(m_Camera.ViewProj());

    m_Gui->BeginFrame();

    ImGui::Begin("Evolution Stats");
    ImGui::Separator();
    ImGui::SliderFloat("Timescale", &m_Timescale, 0.2f, 10.0f);
    ImGui::SliderInt("Agents", &m_NumAgents, 10, 200);
    if (ImGui::Button("Apply")) {
        auto bestDNA = m_Agents[0]->GetCapability("movement")->GetBrain().GetWeightsAndBiases();
        for (auto& a : m_Agents) {
            auto* cap = a->GetCapability("movement");
            if (!cap) continue;
            cap->GetBrain().SetWeightsAndBiases(bestDNA);
        }

        RepositionAgents(true);
        m_IsObserving = true;
    }
    if (ImGui::Button("Train")) {
        m_IsObserving = false;

        if (!m_Trainer) {
            auto eval = std::make_unique<MovementEvaluator>(m_TargetPos);
            m_Trainer = std::make_unique<Trainer>(m_Agents, std::move(eval), "movement");
        }

        constexpr float dt = 0.1f;  // 1/60
        constexpr int steps = 500;
        constexpr float mutationRate = 0.05;
        constexpr float mutationMag = 0.05;
        constexpr float elitism = 0.2;
        for (int g = 0; g < 5; g++, m_GenCount++) {
            if (m_GenCount < 20)
                RepositionAgents(false);
            else
                RepositionAgents(true);
            m_Trainer->RunGeneration(dt, steps, mutationRate, mutationMag, elitism);
            m_AgentView.UpdateInstances(m_Agents);
        }
    }

    ImGui::Text("Generations:  %d", m_GenCount);
    if (m_Trainer) {
        ImGui::Text("Best Fitness: %.3f", m_Trainer->GetBestFitness());
        ImGui::Text("Avg Fitness:  %.3f", m_Trainer->GetAverageFitness());
    }
    ImGui::End();

    m_Gui->EndFrame();
}
