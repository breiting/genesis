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

// Panel width for ImGui
constexpr int ImGuiPanelWidth = 350;

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

    m_Gui = std::make_unique<ImGuiLayer>(ctx.GetWindow().GetNativeWindow());
    ApplyDarkStyle();

    m_Evaluator = std::make_unique<MovementEvaluator>(m_TargetPos);

    // Generate population
    for (int i = 0; i < m_NumAgents; i++) {
        CreateAgent(m_StartPos, m_TargetPos);
    }

    // Viewports
    m_ViewportUi = {0, 0, ImGuiPanelWidth, ctx.GetHeight()};
    m_ViewportWorld = {ImGuiPanelWidth, 0, ctx.GetWidth() - ImGuiPanelWidth, ctx.GetHeight()};
    m_Camera.SetViewportSize(glm::vec2(m_ViewportWorld.w, m_ViewportWorld.h));
    m_Camera.SetPosition(glm::vec2{0.0f});
    m_Camera.FitTo(m_WorldSize);

    m_CanvasView.Init();
    m_AgentView.Init();
    m_StartView.Init();
    m_TargetView.Init();

    onKeyPressed = [this](int key, int /*mods*/) {
        if (key == GLFW_KEY_ESCAPE) {
            m_Quit = true;
        } else if (key == GLFW_KEY_E) {
            float fitness = m_Evaluator->Evaluate(*m_Agents[0].get());
            printf("FITNESS %f\n", fitness);
        } else if (key == GLFW_KEY_O) {
            m_IsObserving = !m_IsObserving;
        } else if (key == GLFW_KEY_S) {
            m_SetStartPos = !m_SetStartPos;
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
                if (m_SetStartPos) {
                    m_StartPos = m_Camera.ScreenToWorld(GetMousePosition());

                    m_Agents.clear();
                    for (int i = 0; i < m_NumAgents; i++) {
                        CreateAgent(m_StartPos, m_TargetPos);
                    }

                    // m_SetStartPos = false;
                }
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

    std::cout << "EvolutionApp initialized." << std::endl;
    return true;
}

void EvolutionApp::Update(gen::AppContext& /*ctx*/, double dt) {
    if (m_IsObserving) {
        for (auto& a : m_Agents) {
            a->Update(dt * m_Timescale);
        }
        m_AgentView.UpdateInstances(m_Agents);
    } else {
        constexpr int steps = 500;
        constexpr float mutationRate = 0.05;
        constexpr float mutationMag = 0.05;
        constexpr float elitism = 0.2;
        for (int g = 0; g < 5; g++, m_GenerationCount++) {
            if (m_GenerationCount < 20)
                RepositionAgents(false);
            else
                RepositionAgents(true);
            if (m_Trainer) {
                m_Trainer->RunGeneration(dt, steps, mutationRate, mutationMag, elitism);
                m_AgentView.UpdateInstances(m_Agents);
            }
        }
        m_IsObserving = true;
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
    m_StartView.Draw(m_StartPos, 2.0f, glm::vec4(1.0, 1.0, 0.0, 1.0), m_Camera.ViewProj());
    m_TargetView.Draw(m_TargetPos, 2.0f, glm::vec4(0.0, 1.0, 1.0, 1.0), m_Camera.ViewProj());
    m_AgentView.Draw(m_Camera.ViewProj());

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
    ImGui::Spacing();

    ImGui::Text("Simulation Parameters");
    ImGui::SliderInt("Population", &m_NumAgents, 5, 200);
    // ImGui::SliderFloat("Mutation Rate", &m_MutationRate, 0.0f, 0.3f, "%.3f");
    // ImGui::SliderFloat("Mutation Mag", &m_MutationMag, 0.0f, 0.3f, "%.3f");
    // ImGui::SliderFloat("Elitism", &m_Elitism, 0.0f, 0.5f, "%.2f");

    ImGui::Spacing();
    ImGui::Text("Training");
    if (ImGui::Button("Init", ImVec2(160, 32))) {
        auto eval = std::make_unique<MovementEvaluator>(m_TargetPos);
        m_Trainer = std::make_unique<Trainer>(m_Agents, std::move(eval), "movement");
        m_IsObserving = false;
        m_GenerationCount = 0;
    }
    ImGui::SameLine();
    if (ImGui::Button("Train", ImVec2(80, 32))) {
        m_IsObserving = false;
    }

    if (ImGui::Button("Observe", ImVec2(120, 32))) {
        m_IsObserving = true;
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (m_Trainer) {
        ImGui::Text("Generation: %d", m_GenerationCount);
        ImGui::Text("Best Fitness: %.3f", m_Trainer->GetBestFitness());
        ImGui::Text("Average Fitness: %.3f", m_Trainer->GetAverageFitness());
        ImGui::ProgressBar(m_Trainer->GetBestFitness(), ImVec2(200, 16));
    }

    ImGui::Text("Cam %s", m_Camera.GetInfo().c_str());
    auto mp = m_Camera.ScreenToWorld(GetMousePosition());
    ImGui::Text("Cur %f %f", mp.x, mp.y);

    // ImGui::SliderFloat("Timescale", &m_Timescale, 0.2f, 10.0f);
    // if (ImGui::Button("Set Start Pos")) {
    //     m_SetStartPos = true;
    // }
    // if (ImGui::Button("LOAD Genome")) {
    //     m_BestGenome.LoadBinary("best.dna");
    // }
    // if (ImGui::Button("SAVE Genome")) {
    //     m_BestGenome = m_Agents[0]->GetCapability("movement")->GetGenome();
    //     m_BestGenome.SaveBinary("best.dna");
    // }
    // if (ImGui::Button("Apply")) {
    //     for (auto& a : m_Agents) {
    //         auto* cap = a->GetCapability("movement");
    //         if (!cap) continue;
    //         cap->SetFromGenome(m_BestGenome);
    //     }
    //
    //     RepositionAgents(true);
    //     m_IsObserving = true;
    // }
    // if (ImGui::Button("Train")) {
    //     m_IsObserving = false;
    //
    //     if (!m_Trainer) {
    //         auto eval = std::make_unique<MovementEvaluator>(m_TargetPos);
    //         m_Trainer = std::make_unique<Trainer>(m_Agents, std::move(eval), "movement");
    //     }
    //
    //     constexpr float dt = 0.1f;  // 1/60
    //     constexpr int steps = 500;
    //     constexpr float mutationRate = 0.05;
    //     constexpr float mutationMag = 0.05;
    //     constexpr float elitism = 0.2;
    //     for (int g = 0; g < 5; g++, m_GenCount++) {
    //         if (m_GenCount < 20)
    //             RepositionAgents(false);
    //         else
    //             RepositionAgents(true);
    //         m_Trainer->RunGeneration(dt, steps, mutationRate, mutationMag, elitism);
    //         m_AgentView.UpdateInstances(m_Agents);
    //     }
    // }
    //
    // ImGui::Text("Generations:  %d", m_GenCount);
    // if (m_Trainer) {
    //     ImGui::Text("Best Fitness: %.3f", m_Trainer->GetBestFitness());
    //     ImGui::Text("Avg Fitness:  %.3f", m_Trainer->GetAverageFitness());
    // }
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
