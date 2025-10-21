#pragma once

#include <genesis/core/IApp.hpp>
#include <genesis/model/Agent.hpp>
#include <genesis/model/Trainer.hpp>
#include <genesis/ui/IGuiLayer.hpp>
#include <genesis/view/AgentView.hpp>
#include <genesis/view/Camera2D.hpp>
#include <memory>

#include "genesis/model/MovementEvaluator.hpp"
#include "genesis/view/TargetView.hpp"

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
    void CreateAgent(const glm::vec2& pos, const glm::vec2& target);
    void RepositionAgents();

   private:
    gen::Camera2D m_Camera;

    std::unique_ptr<gen::IGuiLayer> m_Gui;

    // Mouse handling
    glm::vec2 m_MousePos{0, 0};
    glm::vec2 m_DragStart{0, 0};
    bool m_IsDragging = false;

    float m_Timescale = 1.0f;
    int m_NumAgents = 100;
    int m_GenCount = 0;

    std::unique_ptr<gen::MovementEvaluator> m_Evaluator;

    glm::vec2 m_StartPos;
    glm::vec2 m_TargetPos;

    bool m_IsObserving = false;

    std::vector<std::unique_ptr<gen::Agent>> m_Agents;
    std::unique_ptr<gen::Trainer> m_Trainer;

    // Views
    gen::AgentView m_AgentView;
    gen::TargetView m_StartView;
    gen::TargetView m_TargetView;

    bool m_Quit{false};
};
