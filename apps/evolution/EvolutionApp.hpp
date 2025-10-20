#pragma once

#include <genesis/core/IApp.hpp>
#include <genesis/model/evolution/FitnessTracker.hpp>
#include <genesis/model/evolution/Population.hpp>
#include <genesis/ui/IGuiLayer.hpp>
#include <genesis/view/Camera2D.hpp>

/**
 * @brief Standalone demo app showcasing evolution
 */
class EvolutionApp : public gen::IApp {
   public:
    ~EvolutionApp() = default;
    std::string Name() const override;

    bool Init(gen::AppContext& ctx) override;
    void Update(gen::AppContext& ctx, double dt) override;
    void Render(gen::AppContext& ctx) override;

   private:
    gen::Camera2D m_Camera;

    std::unique_ptr<gen::Population> m_Population;
    std::unique_ptr<gen::FitnessTracker> m_FitnessTracker;

    std::unique_ptr<gen::IGuiLayer> m_Gui;

    glm::vec2 m_MousePos{0, 0};
    glm::vec2 m_DragStart{0, 0};
    bool m_IsDragging = false;

    int m_Generation{0};
    int m_GenerationSteps = 300;
    int m_CurrentStep = 0;
    bool m_Running = true;

    float m_Timescale = 1.0f;
    float m_GlobalWidth = 500.0f;
    float m_GlobalHeight = 500.0f;

    float m_ParticleRadius = 2.0f;
    float m_MaxSpeed = 20.0f;
    int m_NumAgents = 10;

    bool m_Quit{false};
};
