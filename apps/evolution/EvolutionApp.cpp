#include "EvolutionApp.hpp"

#include <GLFW/glfw3.h>
#include <imgui.h>

#include <genesis/core/AppContext.hpp>
#include <genesis/core/Time.hpp>
#include <genesis/core/Window.hpp>
#include <genesis/ui/ImGuiLayer.hpp>
#include <iostream>
#include <memory>

using namespace gen;
using namespace std;

std::string EvolutionApp::Name() const {
    return "EvolutionApp";
}

bool EvolutionApp::Init(gen::AppContext& ctx) {
    std::cout << "Initializing EvolutionApp..." << std::endl;

    std::vector<int> layers = {4, 8, 2};
    m_Population = std::make_unique<Population>(m_NumAgents, layers);
    // m_Evaluator = std::make_unique<PathEvaluator>(50.f, glm::vec2(0, 0));
    m_FitnessTracker = std::make_unique<gen::FitnessTracker>();

    m_Camera.SetOrthoByHeight(m_GlobalHeight, ctx.Aspect());

    // m_View.Init();

    m_Gui = std::make_unique<ImGuiLayer>(ctx.GetWindow().GetNativeWindow());

    m_Camera.SetOrthoByHeight(100, ctx.Aspect());

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
        m_Camera.FitToEnvironment(nullptr, float(w) / float(h));  //
    };

    std::cout << "EvolutionApp initialized." << std::endl;
    return true;
}

void EvolutionApp::Update(gen::AppContext& /*ctx*/, double dt) {
    // m_System->Step(static_cast<float>(dt) * m_Timescale);
    m_CurrentStep++;

    // if (m_CurrentStep > m_GenerationSteps) {
    //     auto fitness = m_Evaluator->Evaluate(*m_System);
    //     m_FitnessTracker->AddGeneration(fitness);
    //     m_Population->SetFitness(fitness);
    //     m_Population->Evolve();
    //
    //     auto& genomes = m_Population->GetGenomes();
    //     auto& particles = m_System->GetParticles();
    //
    //     for (size_t i = 0; i < particles.size(); ++i) {
    //         particles[i]->SetGenome(genomes[i].get());
    //     }
    //
    //     m_System->RandomizePositions();
    //     m_CurrentStep = 0;
    //     m_Generation++;
    // }
    //
    // m_View.SyncWithParticleSystem(m_System.get());
    m_Camera.Update(dt);
}

void EvolutionApp::Render(gen::AppContext& ctx) {
    if (m_Quit) {
        ctx.RequestQuit();
        return;
    }

    // m_View.Draw(m_System.get(), m_Camera);

    m_Gui->BeginFrame();

    ImGui::Begin("Evolution Stats");
    ImGui::Text("Generation: %d", m_Generation);
    ImGui::Text("Best Fitness: %.3f", m_FitnessTracker->GetLastBest());
    ImGui::Text("Avg Fitness: %.3f", m_FitnessTracker->GetLastAverage());
    ImGui::Separator();

    const auto& avg = m_FitnessTracker->GetAverageHistory();
    const auto& best = m_FitnessTracker->GetBestHistory();
    if (!avg.empty()) {
        ImGui::PlotLines("Avg Fitness", avg.data(), (int)avg.size(), 0, nullptr, 0.0f, 1.0f, ImVec2(0, 80));
        ImGui::PlotLines("Best Fitness", best.data(), (int)best.size(), 0, nullptr, 0.0f, 1.0f, ImVec2(0, 80));
    }

    ImGui::Separator();
    ImGui::SliderFloat("Timescale", &m_Timescale, 0.2f, 10.0f);
    ImGui::SliderFloat("Max Speed", &m_MaxSpeed, 5.0f, 100.0f);
    ImGui::End();

    m_Gui->EndFrame();
}
